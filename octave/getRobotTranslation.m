function translation=getRobotTranslation(id)
	if exist('id')
		[~, translation]=getRobotTransform(id);
	else
		[~, translation]=getRobotTransform();
	endif
