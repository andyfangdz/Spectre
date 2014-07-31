import cv2

class Ghost(object):
    def __init__(self, partition):
        mask = cv2.inRange(hsv, np.array((0., 60., 32.)), np.array((180., 255., 255.)))
