function quaternion=getObjectRotation(id)
	if exist('id')
		[quaternion ~]=getObjectTransform(id);
	else
		[quaternion ~]=getObjectTransform();
	endif
