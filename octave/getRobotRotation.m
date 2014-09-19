function quaternion=getRobotRotation(id)
	if exist('id')
		[quaternion ~]=getRobotTransform(id);
	else
		[quaternion ~]=getRobotTransform();
	endif
