#include "EmailForm.h"
#include "Objects2dFactory.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	#include <curl/curl.h>
#endif

string _vfrom;
string _vsubject;
string _vmensage;

char *payload_text[] = {
	NULL
};

char buff[22];
time_t now;
string ttunix;

string _s_to;
string _s_subject;
string _s_mensage;

struct upload_status {
	int lines_read;
};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	CURL *curl;
	CURLcode res;
	struct curl_slist *recipients;
	struct upload_status upload_ctx;
#endif

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
	struct upload_status *upload_ctx = (struct upload_status *)userp;
	const char *data;

	if ((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
		return 0;
	}

	data = payload_text[upload_ctx->lines_read];

	if (data) {
		size_t len = strlen(data);
		memcpy(ptr, data, len);
		upload_ctx->lines_read++;
		return len;
	}
	return 0;
}

EmailForm::EmailForm(string sTitle, string sSend, string sFrom, string sSubject, string sMensage){
	

	CCLayer::init();

	CCSize screenSize = CCDirector::sharedDirector()->getVisibleSize();
	float horizontalPadding = screenSize.width*0.05f;
	float heightHeader = screenSize.height *0.1f;
	float fontSize = heightHeader*0.5f;


	_title   = sTitle;
	_send    = sSend;
	_from    = sFrom;
	_subject = sSubject;
	_mensage = sMensage;

	CCSprite* bg = Objects2dFactory::rectangle(this, ccc3(220,220,220), 0, 0, screenSize.width, screenSize.height, Left, Bottom);

	CCLabelTTF * Lable_Status_Email = CCLabelTTF::create(" ", "TrebuchetMS", fontSize * 2);
	Lable_Status_Email->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	Lable_Status_Email->setColor(ccc3(0,0,0));
	this->addChild(Lable_Status_Email,10,42);


	CCSprite* header = Objects2dFactory::rectangle(this, ccc3(200, 200, 200), 0, screenSize.height - heightHeader, screenSize.width, heightHeader, Left, Bottom, 128);
	CCLabelTTF * Label_Title = Objects2dFactory::label(this, _title, horizontalPadding, screenSize.height - heightHeader + fontSize*0.5f, fontSize, ccc3(0, 15, 0), Left, Bottom, "TrebuchetMS", 0, 0, kCCTextAlignmentLeft, 1);
	
	CCMenu* menuSender = CCMenu::create(NULL);

	btnSender = Objects2dFactory::clickableRectangle(this, menuSender, ccc3(100, 100, 100), 0, 0, screenSize.width / 6, heightHeader, menu_selector(EmailForm::SendEmail),NULL, Left, Bottom);
	CCLabelTTF * Label_Send = Objects2dFactory::label(this, _send, screenSize.width - screenSize.width / 6 + horizontalPadding, screenSize.height - heightHeader + fontSize*0.5f, fontSize, ccc3(0, 15, 0), Left, Bottom, "TrebuchetMS", 0, 0, kCCTextAlignmentLeft, 1);


	menuSender->setPosition(ccp(screenSize.width - screenSize.width / 6, screenSize.height - heightHeader));
	this->addChild(menuSender);

	ScrollableZone * scroll = Objects2dFactory::scrollableZone(this, Vertical, horizontalPadding*0.5f,Bottom, screenSize.width - horizontalPadding*0.5f, screenSize.height*0.9f,0,true,1);

	// Scroll content
	list<CCNode*> scrollContent;

	// Menu
	CCMenu* menu = Objects2dFactory::menu(scroll);

	//Labels
	float RowWidth = 0;
	CCLabelTTF* fromLabel = Objects2dFactory::label(scroll, sFrom, 0, 0, screenSize.height * 0.06f, ccc3(0, 0, 0), Left, Middle, "TrebuchetMS", 0, 0, kCCTextAlignmentLeft, 0);
	if (RowWidth < fromLabel->getContentSize().width)
		RowWidth = fromLabel->getContentSize().width;
	CCLabelTTF* subjectLabel = Objects2dFactory::label(scroll, sSubject, 0, 0, screenSize.height * 0.06f, ccc3(0, 0, 0), Left, Middle, "TrebuchetMS", 0, 0, kCCTextAlignmentLeft, 0);
	if (RowWidth < subjectLabel->getContentSize().width)
		RowWidth = subjectLabel->getContentSize().width;
	CCLabelTTF* mensageLabel = Objects2dFactory::label(scroll, sMensage, 0, 0, screenSize.height * 0.06f, ccc3(0, 0, 0), Left, Middle, "TrebuchetMS", 0, 0, kCCTextAlignmentLeft, 0);
	if (RowWidth < mensageLabel->getContentSize().width)
		RowWidth = mensageLabel->getContentSize().width;
	RowWidth += 20;
	
	from = Objects2dFactory::inputTextField(scroll, " ", RowWidth, 0, screenSize.height * 0.06f, Left, Middle, scroll->getShowedSquareWidth() - RowWidth - horizontalPadding*0.5f, 0, kCCTextAlignmentLeft, 999, ccc3(0, 0, 0), "TrebuchetMS", ccc3(127, 127, 127), false);

	// Delegate for text box
	class UIFromDelegate : public CCTextFieldDelegate {
		virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF* pSender)
		{
			// Set name
			_vfrom = (((TextBox*)pSender)->getString());
			// Returns false, so that detachIME can be called normally
			return false;
		}
	};

	// Set delegate
	from->setDelegate(new UIFromDelegate());

	scrollContent.push_back(from);
	scrollContent.push_back(fromLabel);

	Objects2dHandler::incrementPosition(0, from->getContentSize().height, scrollContent);

	subject = Objects2dFactory::inputTextField(scroll, " ", RowWidth, 0, screenSize.height * 0.06f, Left, Middle, scroll->getShowedSquareWidth() - RowWidth - horizontalPadding*0.5f, 0, kCCTextAlignmentLeft, 999, ccc3(0, 0, 0), "TrebuchetMS", ccc3(127, 127, 127), false);
	// Delegate for text box
	class UISubjectDelegate : public CCTextFieldDelegate {
		virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF* pSender)
		{
			// Set name
			_vsubject = (((TextBox*)pSender)->getString());
			// Returns false, so that detachIME can be called normally
			return false;
		}
	};

	// Set delegate
	subject->setDelegate(new UISubjectDelegate());


	scrollContent.push_back(subject);
	scrollContent.push_back(subjectLabel);
	Objects2dHandler::incrementPosition(0, subject->getContentSize().height, scrollContent);

	mensage = Objects2dFactory::inputAreaField(this, scroll, " ", RowWidth, 0, screenSize.height * 0.06f, scroll->getShowedSquareWidth() - RowWidth - horizontalPadding*0.5f, "TrebuchetMS", kCCTextAlignmentLeft);
	// Delegate for text Area
	class UIMensageDelegate : public CCTextFieldDelegate {
		virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF* pSender)
		{
			// Set name
			_vmensage = (((TextArea*)pSender)->getString());
			// Returns false, so that detachIME can be called normally
			return false;
		}
	};

	// Set delegate
	mensage->setDelegate(new UIMensageDelegate());

	scrollContent.push_back(mensage);
	scrollContent.push_back(mensageLabel);


	Objects2dHandler::incrementPosition(0, mensage->getHeightText(), scrollContent);
	mensage->updateListScroll(scrollContent);

	// First node
	CCNode* firstNode = scrollContent.front();
	float firstNodeTop = firstNode->getPositionY() + (1 - firstNode->getAnchorPoint().y) * firstNode->boundingBox().size.height;

	// Update scrollable zone content height
	scroll->updateContentSize(firstNodeTop);



}

EmailForm::~EmailForm()
{
}

void EmailForm::SendEmail(CCObject * pSender){

	CCLabelTTF * Status = (CCLabelTTF *) this->getChildByTag(42);
	Status->setString("Working...");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if(!system("ping -c1 -s1 www.google.com")){
		
		CCLOG("OK!!!");
		// get time
		now = time(NULL);
		strftime(buff, 22, "[%Y-%m-%d %H:%M:%S]", localtime(&now));
		ttunix = (string)buff;
		CCLOG("OK!!!");
		_s_to = "To: " + _vfrom + "\n";
		_s_subject = "Subject: " + _vsubject + " " + ttunix + "\n";
		_s_mensage = _vmensage +"\n";
		CCLOG("OK!!!");
		payload_text[0] = "Return-Path: root@localhost\n";
		payload_text[1] = (char*)_s_to.c_str();
		payload_text[2] = (char*)_s_subject.c_str();
		payload_text[3] = "\n";
		payload_text[4] = (char*)_s_mensage.c_str();
		payload_text[5] = NULL;


		CCLOG("OK!!!");
		upload_ctx.lines_read = 0;
		CCLOG("OK!!!");
		curl = curl_easy_init();
		CCLOG("OK!!!");
		if (curl)
		{
			
			curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
			curl_easy_setopt(curl, CURLOPT_USE_SSL, 465);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
			curl_easy_setopt(curl, CURLOPT_USERPWD, USERNAME_EMAIL);
			curl_easy_setopt(curl, CURLOPT_PASSWORD, PASSWORD_EMAIL );
			curl_easy_setopt(curl, CURLOPT_MAIL_FROM, NULL);
			recipients = curl_slist_append(recipients, USERNAME_EMAIL);
			curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
			curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
			
			/* send the message (including headers) */
			res = curl_easy_perform(curl);
			/* Check for errors */
			if(res != CURLE_OK){
				fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
			}
			/* free the list of recipients and clean up */
			curl_slist_free_all(recipients);
			/* always cleanup */
			curl_easy_cleanup(curl);
			if (res == 0)
			{
				Status->setString("0 response");
			}
			else
			{
				//sprintf(buffer,"code: %i",res);
				Status->setString("Failed to send...");
			}
		}
		else
		{
			Status->setString("No curl");
		}
	}
#endif
	Status->setString(" ");
	this->setVisible(false);

}