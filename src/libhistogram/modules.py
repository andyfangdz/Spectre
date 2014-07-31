import cv2

class Ghost(object):
    def __init__(self, partition, mask):
        self.mask = mask
        hist = cv2.calcHist(partition, [0], mask, [16], [0, 180])
        cv2.normalize(hist, hist, 0, 255, cv2.NORM_MINMAX)
        self.hist = hist.reshape(-1)

    def getHist(self):
        return self.hist

    def getBackProj(self, frame):
        return cv2.calcBackProject([frame], [0], self.hist, [0, 180], 1)
