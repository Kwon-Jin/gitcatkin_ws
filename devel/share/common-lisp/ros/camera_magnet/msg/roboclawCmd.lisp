; Auto-generated. Do not edit!


(cl:in-package camera_magnet-msg)


;//! \htmlinclude roboclawCmd.msg.html

(cl:defclass <roboclawCmd> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (m1
    :reader m1
    :initarg :m1
    :type cl:fixnum
    :initform 0)
   (m2
    :reader m2
    :initarg :m2
    :type cl:fixnum
    :initform 0)
   (x1des
    :reader x1des
    :initarg :x1des
    :type cl:float
    :initform 0.0)
   (x2des
    :reader x2des
    :initarg :x2des
    :type cl:float
    :initform 0.0))
)

(cl:defclass roboclawCmd (<roboclawCmd>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <roboclawCmd>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'roboclawCmd)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name camera_magnet-msg:<roboclawCmd> is deprecated: use camera_magnet-msg:roboclawCmd instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <roboclawCmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader camera_magnet-msg:header-val is deprecated.  Use camera_magnet-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'm1-val :lambda-list '(m))
(cl:defmethod m1-val ((m <roboclawCmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader camera_magnet-msg:m1-val is deprecated.  Use camera_magnet-msg:m1 instead.")
  (m1 m))

(cl:ensure-generic-function 'm2-val :lambda-list '(m))
(cl:defmethod m2-val ((m <roboclawCmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader camera_magnet-msg:m2-val is deprecated.  Use camera_magnet-msg:m2 instead.")
  (m2 m))

(cl:ensure-generic-function 'x1des-val :lambda-list '(m))
(cl:defmethod x1des-val ((m <roboclawCmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader camera_magnet-msg:x1des-val is deprecated.  Use camera_magnet-msg:x1des instead.")
  (x1des m))

(cl:ensure-generic-function 'x2des-val :lambda-list '(m))
(cl:defmethod x2des-val ((m <roboclawCmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader camera_magnet-msg:x2des-val is deprecated.  Use camera_magnet-msg:x2des instead.")
  (x2des m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <roboclawCmd>) ostream)
  "Serializes a message object of type '<roboclawCmd>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let* ((signed (cl:slot-value msg 'm1)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'm2)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'x1des))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'x2des))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <roboclawCmd>) istream)
  "Deserializes a message object of type '<roboclawCmd>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'm1) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'm2) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'x1des) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'x2des) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<roboclawCmd>)))
  "Returns string type for a message object of type '<roboclawCmd>"
  "camera_magnet/roboclawCmd")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'roboclawCmd)))
  "Returns string type for a message object of type 'roboclawCmd"
  "camera_magnet/roboclawCmd")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<roboclawCmd>)))
  "Returns md5sum for a message object of type '<roboclawCmd>"
  "aa305f06e458a2bc9349c21cb907612f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'roboclawCmd)))
  "Returns md5sum for a message object of type 'roboclawCmd"
  "aa305f06e458a2bc9349c21cb907612f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<roboclawCmd>)))
  "Returns full string definition for message of type '<roboclawCmd>"
  (cl:format cl:nil "# Current command sent to roboclaw with timestamp~%~%Header header~%int16 m1~%int16 m2~%float64 x1des~%float64 x2des~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.secs: seconds (stamp_secs) since epoch~%# * stamp.nsecs: nanoseconds since stamp_secs~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'roboclawCmd)))
  "Returns full string definition for message of type 'roboclawCmd"
  (cl:format cl:nil "# Current command sent to roboclaw with timestamp~%~%Header header~%int16 m1~%int16 m2~%float64 x1des~%float64 x2des~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.secs: seconds (stamp_secs) since epoch~%# * stamp.nsecs: nanoseconds since stamp_secs~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <roboclawCmd>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     2
     2
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <roboclawCmd>))
  "Converts a ROS message object to a list"
  (cl:list 'roboclawCmd
    (cl:cons ':header (header msg))
    (cl:cons ':m1 (m1 msg))
    (cl:cons ':m2 (m2 msg))
    (cl:cons ':x1des (x1des msg))
    (cl:cons ':x2des (x2des msg))
))
