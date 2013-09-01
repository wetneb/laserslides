
bb:black_board.cpp viewport.cpp
	g++ $^ -lopencv_core -lopencv_imgproc -lopencv_calib3d -lopencv_highgui -lopencv_features2d -o $@

