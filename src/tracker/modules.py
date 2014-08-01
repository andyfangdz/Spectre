'''
    The wrapper for camshift algorithm in OpenCV.
'''
import cv2

PENDING = 0
TRACKING = 1
MISSING = 2

class Tracker(object):
    '''
    The wrapper for camshift algorithm in OpenCV.
    '''
    def __init__(self, sizeTreshold=4):
        self.state = PENDING
        self.trackingWindow = None
        self.hist = None
        self.sizeTreshold = sizeTreshold
        self.lastWindow = None

    def setState(self, state):
        '''
        This method sets the state of the tracker.
        '''
        if state in [PENDING, TRACKING, MISSING]:
            self.state = state

    def getState(self):
        '''
        This method returns the state of the tracker.
        '''
        if self.state is not None:
            return self.state

    def initialize(self, selection, hist):
        '''
        This method initializes the tracker with supplied
        histogram and tracking box, and will set the state to tracking.
        '''
        x0, y0, x1, y1 = selection
        self.trackingWindow = (x0, y0, x1-x0, y1-y0)
        self.hist = hist
        self.setState(TRACKING)

    def windowSize(self):
        '''
        This method calculates the size of tracking window.
        '''
        return abs(self.trackingWindow[2] * self.trackingWindow[3])

    def tick(self, frame, additionalMask = None):
        '''
        This method should be executed every frame.
        '''
        if additionalMask:
            frame &= additionalMask
        term_crit = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 1)
        trackBox, self.trackingWindow = cv2.CamShift(
            frame, self.trackingWindow, term_crit)
        if self.windowSize() <= self.sizeTreshold:
            self.setState(MISSING)
            if self.lastWindow is not None:
                self.trackingWindow = self.lastWindow
        else:
            self.setState(TRACKING)
            self.lastWindow = self.trackingWindow
        return self.state, trackBox[0]
