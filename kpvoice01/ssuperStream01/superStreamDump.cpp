#include "superStream.h"

void _superStreamBase::_ssDumpSelf( ) {
    _prEFn( "" );
    _prEFn( "_ssErrAction   '%d'" , _ssErrAction    ) ;
    _prEFn( "_ssDir         '%d'" , _ssDir          ) ;
    _prEFn( "_ssType        '%d'" , _ssType         ) ;
    _prEFn( "_ssPath        '%s'" , _ssPath         ) ;
    _prEFn( "_ssComment     '%s'" , _ssComment      ) ;
    _prEFn( "_ssOK          '%p'" , _ssOK           ) ;
    _prEFn( "_ssFD          '%d'" , _ssFD           ) ;

    _prEFn( "_ssInfoW " 
            " tryCnt,Len  %4lld %4lld " 
            " skipCnt,Len %4lld %4lld " 
            " succCnt,Len %4lld %4lld " 
            , _ssInfoW . _tryCnt     
            , _ssInfoW . _tryLen     
            , _ssInfoW . _skipCnt    
            , _ssInfoW . _skipLen    
            , _ssInfoW . _succCnt    
            , _ssInfoW . _succLen    
            ) ;

    _prEFn( "_ssInfoR " 
            " tryCnt,Len  %4lld %4lld " 
            " skipCnt,Len %4lld %4lld " 
            " succCnt,Len %4lld %4lld " 
            , _ssInfoR . _tryCnt     
            , _ssInfoR . _tryLen     
            , _ssInfoR . _skipCnt    
            , _ssInfoR . _skipLen    
            , _ssInfoR . _succCnt    
            , _ssInfoR . _succLen    
            ) ;
    _prEFn( "" );

} /* _superStreamBase::_ssDumpSelf */

