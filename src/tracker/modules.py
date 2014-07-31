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
    def __init__(self, size_treshold=4):
        self.state = PENDING
        self.tracking_window = None
        self.hist = None
        self.size_treshold = size_treshold

    def set_state(self, state):
        '''
        This method sets the state of the tracker.
        '''
        if state in [PENDING, TRACKING, MISSING]:
            self.state = state

    def get_state(self):
        '''
        This method returns the state of the tracker.
        '''
        if self.state:
            return self.state

    def initialize(self, selection, hist):
        '''
        This method initializes the tracker with supplied
        histogram and tracking box, and will set the state to tracking.
        '''
        self.tracking_window = selection
        self.hist = hist
        self.set_state(TRACKING)

    def window_size(self):
        '''
        This method calculates the size of tracking window.
        '''
        return abs(self.tracking_window[2] * self.tracking_window[3])

    def tick(self, frame, additional_mask = None):
        '''
        This method should be executed every frame.
        '''
        if additional_mask:
            frame &= additional_mask
        term_crit = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 1)
        track_box, self.tracking_window = cv2.CamShift(
            frame, self.tracking_window, term_crit)
        if self.window_size() <= self.size_treshold:
            self.set_state(MISSING)
        else:
            self.set_state(TRACKING)
        return self.state, track_box[0]
