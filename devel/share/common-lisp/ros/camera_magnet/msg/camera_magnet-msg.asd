
(cl:in-package :asdf)

(defsystem "camera_magnet-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "xyReal" :depends-on ("_package_xyReal"))
    (:file "_package_xyReal" :depends-on ("_package"))
    (:file "roboclawCmd" :depends-on ("_package_roboclawCmd"))
    (:file "_package_roboclawCmd" :depends-on ("_package"))
  ))