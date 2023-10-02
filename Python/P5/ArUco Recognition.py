import cv2
import numpy as np
import pyrealsense2 as rs

# Create an ArUco detector object
aruco = cv2.aruco

# Define the dictionary and parameters
dictionary = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_ARUCO_ORIGINAL)
parameters =  cv2.aruco.DetectorParameters()
detector = cv2.aruco.ArucoDetector(dictionary, parameters)

# Realsense
# Create a pipeline
#pipeline = rs.pipeline()
#
## Configure the pipeline for color and depth streams
#config = rs.config()
#config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)
#config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
#
## Start the pipeline
#pipeline.start(config)
#


#def printDimension(MarkerCorners):
#    try:
#        listed = MarkerCorners[0].tolist()
#        TLX = listed[0][0][0]
#        TRX = listed[0][2][0]
#        BRY = listed[0][2][1]
#        TRY = listed[0][1][1]
#        pixelWidth = np.abs(TRX-TLX)
#        pixelHeight = np.abs(BRY-TRY)
#        print(f"height: {pixelHeight}")
#        print(f"width: {pixelWidth}")
#    except:
#        print("No ArUco Visible")

# Open the camera
# # Use 0 for the default camera
cap = cv2.VideoCapture(2)
while True:
    ret, frame = cap.read()
    # Wait for the next frame
    #frames = pipeline.wait_for_frames()
#
    ## Get the color and depth frames
    #color_frame = frames.get_color_frame()
    #depth_frame = frames.get_depth_frame()
#
    #if not color_frame or not depth_frame:
    #    continue
#
    ## Convert RealSense frames to OpenCV format
    #color_image = np.asanyarray(color_frame.get_data())
    #depth_image = np.asanyarray(depth_frame.get_data())
#
    ## Perform image processing using OpenCV
    ## (e.g., object detection, face recognition, etc.)

    # Display the processed frame
    #cv2.imshow("RealSense Frame", color_image)
    #
    #if not ret:
    #    break
#
    ## Detect ArUco markers
    #markerCorners, markerIds, rejectedCandidates = detector.detectMarkers(frame)
#
    #if markerIds is not None:
    #    # Draw the detected markers and their IDs
    #    aruco.drawDetectedMarkers(frame, markerCorners, markerIds)
    #    #printDimension(markerCorners)
    ## Display the frame
    cv2.imshow("ArUco Detection", frame)
#
    ## Exit the loop when the 'q' key is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the camera and close all OpenCV windows
#cap.release()

# Realsense
#pipeline.stop()
cv2.destroyAllWindows()