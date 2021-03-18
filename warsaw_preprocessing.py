import cv2
import os
import face_recognition
import shutil
from tqdm import tqdm
import time
from multiprocessing import Pool
from functools import partial
import exiftool


def crop_face(image):
    """"
    function for cropping face from image with its surreandings
    coefs was chosen empirically so that resulting picture is square
    and the eyes are in the middle of it
    """
    face_landmarks = face_recognition.face_landmarks(image)
    if np.all(face_landmarks == 0):
        return np.empty(shape=(0, 0))
    elif len(face_landmarks) > 0:
        landmarks = [x for v in face_landmarks[0].values() for x in v]
        x,y,w,h = cv2.boundingRect(np.asarray(landmarks))
        return np.array([int(y-0.7*h), int(y+1.1*h), int(x+w/2-0.9*h), int(x+w/2+0.9*h)])
    return np.empty(shape=(0, 0))

def is_all_black(image_path):
    img = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)
    return np.mean(img) < 6

def count_timestamp(path):
    """
    Count time of file creation as seconds after midnight
    """
    with exiftool.ExifTool() as et:
        metadata = et.get_metadata(path)
    create_time = metadata['Composite:SubSecCreateDate']
    splited = create_time.split()[1].split(':')
    timestamp = (float(splited[0]) * 60 + float(splited[1])) * 60 + float(splited[2])
    return timestamp

def parse_photos(in_dir, out_dir, crop=False):
    
    print('Pairing is about to start!')
    temp_dir = f'{out_dir}_temp'
    if not os.path.exists(temp_dir):
        os.makedirs(temp_dir)
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)
    
    for root, dirs, files in os.walk(in_dir, topdown=False):
        sorted_files = sorted(files)
        paths = [os.path.join(root, name) for name in sorted_files if '.JPG' in name]
        test = False
        num_blacks = 0
        last_timestamp = 0
        global_index = 0
        file = root.replace(in_dir, '')
        print(file)
        if not os.path.exists(os.path.join(temp_dir, file)):
            os.makedirs(os.path.join(temp_dir, file))
        if not os.path.exists(os.path.join(out_dir, file)):
            os.makedirs(os.path.join(out_dir, file))
        for path in tqdm(paths):        
            if is_all_black(path):
                num_blacks += 1
                if num_blacks == 2:
                    test = False
                elif num_blacks == 3:
                    test = True
            else:
                if not test:
                    num_blacks = 0
                    timestamp = count_timestamp(path)
                    if (timestamp - last_timestamp) < 1:
                        old_incorrect_path = path
                        new_incorrect_path = os.path.join(temp_dir, file, f'{global_index}.jpg')
                        old_correct_path = good_path
                        new_correct_path = os.path.join(temp_dir, file, f'{global_index}c.jpg')
                        shutil.copy(old_incorrect_path, new_incorrect_path)
                        shutil.copy(old_correct_path, new_correct_path)
                        global_index += 1
                    else:
                        good_path = path
                    last_timestamp = timestamp
                num_blacks = 0    
        parse_pool = partial(parse_one_photo, crop=crop, temp_dir=os.path.join(temp_dir, file), out_dir=os.path.join(out_dir, file))
        pool = Pool()     
        pool.map(parse_pool, range(global_index))
        
def parse_one_photo(index, crop, temp_dir, out_dir):
    new_correct_path = os.path.join(out_dir, f'{index}c.jpg')
    old_correct_path = os.path.join(temp_dir, f'{index}c.jpg')
    new_incorrect_path = os.path.join(out_dir, f'{index}.jpg')
    old_incorrect_path = os.path.join(temp_dir, f'{index}.jpg')
    if crop:
        image = cv2.imread(old_correct_path)
        coords = crop_face(image)
        if coords.size > 0:
            cropped_image = image[coords[0]:coords[1], coords[2]:coords[3]]
            cv2.imwrite(old_correct_path, cropped_image)
            image = cv2.imread(old_incorrect_path)
            cropped_image = image[coords[0]:coords[1], coords[2]:coords[3]]
            cv2.imwrite(old_incorrect_path, cropped_image)
        
if __name__ == "__main__":
    """
    python3 -m photoaid.preprocessing.warsaw_preprocessing --in_dir /data_input_folder \
        --out_dir /data_output_folder --crop True
    """
    start_time = time.time()
    fire.Fire(parse_photos)
    print("--- %s seconds ---" % (time.time() - start_time))
