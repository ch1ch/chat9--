//
//  SquareHomeLayer.h
//  chats
//
//  Created by 廖丹 on 13-8-27.
//
//

#ifndef __chats__SquareHomeLayer__
#define __chats__SquareHomeLayer__

#include <iostream>
#include "BaseHomeLayer.h"

class SquareHomeLayer: public BaseHomeLayer
{
private:
	
public:
    
	virtual bool init ();
    
	CREATE_FUNC (SquareHomeLayer);
};

#endif /* defined(__chats__SquareHomeLayer__) */
