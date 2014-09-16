% Example for robonautFlask.xml

nRobot=size(getRobotName,1)
% First Robot pose
initPoseRobot=getRobotTransform(1)
initQuatRobot=initPoseRobot(1:4)
initTranRobot=initPoseRobot(5:7)

nObject=size(getObjectTransform,1)
% First object pose
initPoseObject=getObjectTransform(1)
initQuatObject=initPoseObject(1:4)
initTranObject=initPoseObject(5:7)

% translate object +9 in Z axis relate to world frame
newTranObject=initTranObject+[9;0;0]
setObjectTransform([initQuatObject ; newTranObject])

% q is represent rotation pi/2 radian around X axis
q=rot2q([1;0;0],pi/2)
% rot object using q
newQ=q*quaternion(initQuatObject(1),initQuatObject(2),initQuatObject(3),initQuatObject(4))
% set q rotation
setObjectTransform([[newQ.w newQ.x newQ.y newQ.z]' ; newTranObject])

