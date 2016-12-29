/*
 * Code taken, and modified, from MSDN tutorial
 * https://code.msdn.microsoft.com/windowsdesktop/sending-toast-notifications-71e230a2/
 */

#pragma once

using namespace Microsoft::WRL;
using namespace ABI::Windows::UI::Notifications;
using namespace ABI::Windows::Data::Xml::Dom;
using namespace Windows::Foundation;

class windows_notification {
	static HRESULT SetImageSrc(wchar_t* imagePath, IXmlDocument* toastXml);
	static HRESULT SetTextValues(wchar_t** textValues, UINT32 textValuesCount, UINT32* textValuesLengths, IXmlDocument* toastXml);
	static HRESULT SetNodeValueString(HSTRING inputString, IXmlNode* node, IXmlDocument* xml);
	static HRESULT CreateToast(IToastNotificationManagerStatics* toastManager, IXmlDocument* xml);
	static HRESULT InstallShortcut(wchar_t* shortcutPath);
	static HRESULT CreateToastXml(_In_ IToastNotificationManagerStatics *toastManager, _Outptr_ IXmlDocument** inputXml, _In_ wchar_t const* image, _In_ wchar_t *text[]);
public:
	static HRESULT TryCreateShortcut();
	static HRESULT DisplayToast(wchar_t const* image, wchar_t** text);
};