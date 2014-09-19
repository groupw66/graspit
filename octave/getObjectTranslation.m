function translation=getObjectTranslation(id)
	if exist('id')
		[~, translation]=getObjectTransform(id);
	else
		[~, translation]=getObjectTransform();
	endif
