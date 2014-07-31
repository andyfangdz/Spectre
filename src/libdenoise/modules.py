'''
Define the operations used to denoise the image.
'''
import cv2
import numpy as np

def denoise(frame, useMorphOps = True, useGaussianBlur = True):
    if useMorphOps:
        kernel = np.ones((5,5),np.uint8)
        frame = cv2.morphologyEx(frame, cv2.MORPH_OPEN, kernel)
        frame = cv2.morphologyEx(frame, cv2.MORPH_CLOSE, kernel)
    if useGaussianBlur:
        frame = cv2.GaussianBlur(frame, (5, 5), 0)
    return frame
