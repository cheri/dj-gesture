**DJ Gesture Recognition with Microsoft Kinect**

This project requires openCV to have been installed.

How to download the project from github:

git clone https://github.com/navadavuluri/dj-gesture.git

(download should happen to a directory named "dj-gesture" by default)

cd dj-gesture

(for compiling)
g++ -g kmeans.h kmeans.cpp get_point_centroids.cpp get_xyz_data.cpp get_point_clusters.cpp prior_transition_matrix.cpp pr_hmm.cpp main.cpp `pkg-config --cflags --libs opencv`


(for running the project for testing)
a.out t
(for playing around with the binary and getting an audio response)
a.out p
