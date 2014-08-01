'''
The actuall camshift runtime.
'''
import cv2
import numpy as np

from libcv import video
from utils import mark
from libdenoise.modules import denoise
from libhistogram.modules import Ghost
from tracker import modules as Shifter


DRAGING = 1
PENDING = 0

class TrackingDevice(object):
    def __init__(self, videoSrc = 0):
        self.videoSrc= videoSrc
        self.cam = video.create_capture(videoSrc)
        self.mainWindow = "camshift " + str(videoSrc)
        self.tracker = Shifter.Tracker()
        self.ghost = Ghost()
        self.selection = None
        self.readyForDeployment = False
        self.mouseState = None

        cv2.namedWindow(self.mainWindow)
        cv2.setMouseCallback(self.mainWindow, self.onMouse)

    def onMouse(self, event, x, y, flags, param):
        x, y = np.int16([x, y]) # Bypass OpenCV Bug -> B.O.B :)
        if event == cv2.EVENT_LBUTTONDOWN:
            self.dragStart = (x, y)
            self.tracker.setState(Shifter.PENDING)
            self.mouseState = DRAGING
        elif event == cv2.EVENT_MOUSEMOVE:
            if self.mouseState == DRAGING:
                x0, y0 = np.maximum(0, np.minimum(self.dragStart, [x,y]))
                x1, y1 = np.minimum(self.frame.shape[:2], np.maximum(self.dragStart, [x,y]))
                if x1 > x0 and y1 > y0:
                    self.selection = (x0, y0, x1, y1)
        elif event == cv2.EVENT_LBUTTONUP:
            if self.selection is not None:
                self.readyForDeployment = True
            self.mouseState = PENDING
            self.dragStart = None


    def tick(self):
        ret, self.frame = self.cam.read()
        visual = self.frame.copy()
        hsv = cv2.cvtColor(self.frame, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, np.array((0., 60., 32.)), np.array((180., 255., 255.)))
        if self.selection is not None:
            x0, y0, x1, y1 = self.selection
            hsv_roi = hsv[y0:y1, x0:x1]
            mask_roi = mask[y0:y1, x0:x1]
            vis_roi = visual[y0:y1, x0:x1]
            cv2.bitwise_not(vis_roi, vis_roi)
            self.ghost.update(hsv_roi, mask_roi)
            if self.readyForDeployment == False:
                cv2.imshow(self.mainWindow +' Histogram', self.ghost.showHist())
            if self.readyForDeployment:
                self.tracker.initialize(self.selection, self.ghost.getHist())
                self.selection = None
                self.readyForDeployment = False
        if self.tracker.getState() != Shifter.PENDING:
            bimg = self.ghost.getBackProj(hsv) & mask
            bimg = denoise(bimg)
            state, coord = self.tracker.tick(bimg)
            visual[:] = bimg[..., np.newaxis]
            if self.tracker.getState() == Shifter.TRACKING:
                mark.draw_machine_mark(60, coord, visual)
        cv2.imshow(self.mainWindow, visual)

a = TrackingDevice(1)
b = TrackingDevice(0)
while True:
    a.tick()
    b.tick()
    ch = 0xFF & cv2.waitKey(5)
    if ch == 27:
        break
