TopQX_Email
===========

TopQX_Email is a library made in C++/Cocos2d-x.

Library to help the creation and sending an email for Win, iOS, Mac and Linux.

This library was developed under project TOPQX, and received funds from Programa Operacional Regional do Centro: CENTRO-07-0202-FEDER-022839


NOTE: Uses [TopQX_2D] (https://github.com/Cnotinfor/TopQX_2D) for  the interface.

Example: 
-----------

```
#include "EmailForm.h"

 CCMenuItemImage *btOpenEmail = CCMenuItemImage::create(
                                        "EmailNormal.png",
                                        "EmailSelected.png",
                                        this,
                                        menu_selector(HelloWorld::openEmailFrom));
										
 CCMenu* pMenu = CCMenu::create(btEmail, NULL);
 
 	emailForm = new EmailForm();
	this->addChild(emailForm, 10,73);
	emailForm->setVisible(false);
```

```
// function buton Open Email from.

void HelloWorld::openEmailFrom(CCObject* pSender)
{
	this->getChildByTag(73)->setVisible(true);
}
```