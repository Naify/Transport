unix:{
  
    COMMAND = echo "eureca_sign not installed"
        
    exists(/usr/bin/eureca_sign) {
        COMMAND = sudo eureca_sign -d $$OUT_PWD/$$DESTDIR
    }

    QMAKE_POST_LINK += $$COMMAND

}
