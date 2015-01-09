
(cl:in-package :asdf)

(defsystem "camera_magnet-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "xyReal" :depends-on ("_package_xyReal"))
    (:file "_package_xyReal" :depends-on ("_package"))
  ))