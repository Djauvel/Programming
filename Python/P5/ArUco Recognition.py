import cv2
import numpy as np

# Constants
markerSizeCM = 12

#Calibrate camera to obtain camera matrix and 





# Initialize the webcam
cap = cv2.VideoCapture(0)

# Load the ArUco dictionary
aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_ARUCO_ORIGINAL)

# Create a marker detector
aruco_params =  cv2.aruco.DetectorParameters()
detector = cv2.aruco.ArucoDetector(aruco_dict, aruco_params)

def printDimension(MarkerCorners):
    try:
        listed = MarkerCorners[0].tolist()
        TLX = listed[0][0][0]
        TRX = listed[0][2][0]
        BRY = listed[0][2][1]
        TRY = listed[0][1][1]
        pixelWidth = np.abs(TRX-TLX)
        pixelHeight = np.abs(BRY-TRY)
        print(f"height: {pixelHeight}")
        print(f"width: {pixelWidth}")
    except:
        print("No ArUco Visible")



while True:
    # Capture a frame from the webcam
    ret, frame = cap.read()
    
    if not ret:
        break
    
    # Detect markers in the frame
    corners, ids, _ = detector.detectMarkers(frame)
    
    if ids is not None:
        # Draw detected markers on the frame
        cv2.aruco.drawDetectedMarkers(frame, corners, ids)

        # Estimate pose of found ArUco markers
        # Print marker IDs
        for i in range(len(ids)):
            print(f"Marker ID: {ids[i]}")
    
    # Display the frame with markers
    cv2.imshow('ArUco Marker Detection', frame)
    printDimension(corners)
    
    # Exit the loop by pressing 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the webcam and close all OpenCV windows
cap.release()
cv2.destroyAllWindows()