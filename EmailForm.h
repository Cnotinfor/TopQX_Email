#ifndef EMAIL_FORM_H
#define EMAIL_FORM_H

#include "cocos2d.h"
#include <string.h>
#include "EmailCredentials.h"
#include "TextBox.h"
#include "TextArea.h"

using namespace std;
using namespace cocos2d;

class EmailForm: public CCLayer
{
public:
	EmailForm(string sTitle = "Support", string sSend = "Send ", string sFrom = "From: ", string sSubject = "Subject: ", string sMensage = "Mensage: ");
	
	~EmailForm();

	void SendEmail(CCObject* pSender);

	TextBox* from;
	TextBox* subject;
	TextArea* mensage;
	CCMenuItemImage* btnSender;

private:
	string _title;
	string _send;
	string _from;
	string _subject;
	string _mensage;

};

#endif // !EMAIL_FORM_H
