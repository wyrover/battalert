//***************************************************************************
//
// [ nCore ]
//
// Copyright (c) 2001-2008 Jean-Charles Lefebvre <jcl ATNOSPAM jcl DOT name>
//
// This file is part of nCore.
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions :
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
//***************************************************************************

#ifndef __nCore_NetSmtpSender_Header__
#define __nCore_NetSmtpSender_Header__
#if defined(NCORE_ENABLE_NETWORKING) && defined(NCORE_ENABLE_NETWORKING_SMTP_SENDER)
namespace nCore {


//---------------------------------------------------------------------------
// NetSmtpSender
//---------------------------------------------------------------------------
class NCORE_EXPORT NetSmtpSender
{
public :
  typedef std::pair<StringA,StringA>  Recipient;
  typedef std::pair<StringA,StringA>  Attachment;

  typedef std::vector<Recipient>      Recipients;
  typedef Recipients::iterator        RecipientsIt;
  typedef Recipients::const_iterator  RecipientsCIt;

  typedef std::vector<Attachment>      Attachments;
  typedef Attachments::iterator        AttachmentsIt;
  typedef Attachments::const_iterator  AttachmentsCIt;


public :
  NetSmtpSender (void);
  ~NetSmtpSender (void);

  // optional settings
  void SetAgent (const StringA& strAgent); // this will change the default value of the "User-Agent:" header
  void SetLog   (bool bEnableLogOutput);   // default is false

  // setup smtp server
  void SetServer (const NetAddress& naServer);

  // setup mail data
  void SetFrom       (const StringA& strEMail, const StringA& strName="");
  void AddTo         (const StringA& strEMail, const StringA& strName="");
  void AddCc         (const StringA& strEMail, const StringA& strName="");
  void AddBcc        (const StringA& strEMail, const StringA& strName="");
  void SetSubject    (const StringA& strSubject);
  void SetMessage    (const StringA& strMessage);
  void AddAttachment (const StringA& strFilePath, const StringA& strDisplayFileName="");

  // clear mail data
  // * clears the from/to/cc/bcc recipients, subject, body, attachements
  //   but not the server address.
  // * useful for sending another e-mail
  void ClearMailData (void);

  // send mail
  bool SendMail (int nTimeoutMS=-1); // remember, this call is blocking


private :
  // utils
  bool SendAndGetResponse (NetSocket* pSocket, int nTimeoutMS, const StringA& strDataToSend, StringA& strOutResponse) const;
  void AppendHeaders      (StringA& strOut);
  void AppendBody         (StringA& strOut);

  // static utils
  static void CleanupRecipient          (Recipient& recipient);
  static void AppendRecipientToString   (StringA& strOut, const NetSmtpSender::Recipient& recipient, bool bAddressOnly=false);
  static void BuildRandomBoundaryString (StringA& strOut);
  static void AttachFileToBody          (StringA& strOut, const Attachment& file, const StringA& strBoundary, const StringA& strNL);


private :
  // settings
  StringA m_strAgent;
  bool    m_bLog;

  // server
  NetAddress m_naServer;

  // mail data
  Recipient   m_From;
  Recipients  m_To;
  Recipients  m_Cc;
  Recipients  m_Bcc;
  StringA     m_strSubject;
  StringA     m_strMessage;
  Attachments m_Attachments;
};


#include "NetSmtpSender.inl.h"


}
#endif // #if defined(NCORE_ENABLE_NETWORKING) && defined(NCORE_ENABLE_NETWORKING_SMTP_SENDER)
#endif // #ifndef __nCore_NetSmtpSender_Header__
