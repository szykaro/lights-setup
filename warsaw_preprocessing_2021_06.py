import numpy as np
import os
import cv2
import fire
import face_recognition
from multiprocessing import Pool
from functools import partial
import time


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
        return np.array([int(y-1*h), int(y+1.4*h), int(x+w/2-1.2*h), int(x+w/2+1.2*h)])
    return np.empty(shape=(0, 0))

def parse_photos(in_dir, out_dir, workers=2):   
    print('Pairing is about to start!')
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)
    
    for root, dirs, files in os.walk(in_dir, topdown=False):
        sorted_files = sorted(files)
        paths = [os.path.join(root, name) for name in sorted_files if '_A.' in name]
        file = root.replace(in_dir, '')
        if len(file) != 0 and file[0] == '/':
            file = file[1:]
        print(file)
        if not os.path.exists(os.path.join(out_dir, file)):
            os.makedirs(os.path.join(out_dir, file))
        parse_pool = partial(parse_one_photo, in_dir=os.path.join(in_dir, file), 
                             out_dir=os.path.join(out_dir, file))
        pool = Pool(processes=workers)     
        pool.map(parse_pool, paths)
        
def parse_one_photo(path, in_dir, out_dir):
    old_path = path
    new_path = path.replace(in_dir, out_dir)    
    image = cv2.imread(old_path)
    coords = crop_face(image)
    if coords.size > 0:
        cropped_image = get_cropped_image(image, coords)
        cv2.imwrite(new_path, cropped_image)
        old_path = old_path.replace('_A.', '_B.')
        new_path = new_path.replace('_A.', '_B.')
        if os.path.isfile(old_path):
            image = cv2.imread(old_path)
            cropped_image = get_cropped_image(image, coords)
            cv2.imwrite(new_path, cropped_image)
        old_path = old_path.replace('_B.', '_C.')
        new_path = new_path.replace('_B.', '_C.')
        if os.path.isfile(old_path):
            image = cv2.imread(old_path)
            cropped_image = get_cropped_image(image, coords)
            cv2.imwrite(new_path, cropped_image)
            
def get_cropped_image(image, coords):
    y, x, _ = image.shape
    bordered_image = cv2.copyMakeBorder(image, 
                                        max(-coords[0], 0), 
                                        max(coords[1] - y, 0),
                                        max(-coords[2], 0),
                                        max(coords[3] - x, 0),
                                        cv2.BORDER_CONSTANT,
                                        value=[255, 255, 255])
    new_indexes = (max(coords[0], 0), max(coords[2], 0))  
    return bordered_image[new_indexes[0]:coords[1]+max(-coords[0], 0), new_indexes[1]:coords[3]+max(-coords[2], 0)]
        
if __name__ == "__main__":
    """
    python3 -m warsaw_preprocessing_2021_06 --in_dir /data/input_folder --out_dir /data/output_folder --workers num_of_workers
    """
    start_time = time.time()
    fire.Fire(parse_photos)
    print("--- %s seconds ---" % (time.time() - start_time))
