import cv2
import numpy as np

class Ghost(object):
    def __init__(self):
        self.mask = None
        self.hist = None

    def update(self, partition, mask):
        self.mask = mask
        hist = cv2.calcHist([partition], [0], mask, [16], [0, 180])
        cv2.normalize(hist, hist, 0, 255, cv2.NORM_MINMAX)
        self.hist = hist.reshape(-1)

    def getHist(self):
        return self.hist

    def showHist(self):
        bin_count = self.hist.shape[0]
        bin_w = 24
        img = np.zeros((256, bin_count*bin_w, 3), np.uint8)
        for i in xrange(bin_count):
            h = int(self.hist[i])
            cv2.rectangle(img, (i*bin_w+2, 255), ((i+1)*bin_w-2, 255-h), (int(180.0*i/bin_count), 255, 255), -1)
        img = cv2.cvtColor(img, cv2.COLOR_HSV2BGR)
        return img

    def getBackProj(self, frame):
        return cv2.calcBackProject([frame], [0], self.hist, [0, 180], 1)
