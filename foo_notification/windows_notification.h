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
	ComPtr<IToastNotification> prevToast = nullptr;

	HRESULT SetImageSrc(wchar_t* imagePath, IXmlDocument* toastXml);
	HRESULT SetTextValues(wchar_t** textValues, UINT32 textValuesCount, UINT32* textValuesLengths, IXmlDocument* toastXml);
	HRESULT SetNodeValueString(HSTRING inputString, IXmlNode* node, IXmlDocument* xml);
	HRESULT CreateToast(IToastNotificationManagerStatics *toastManager, IXmlDocument *xml);
	HRESULT InstallShortcut(wchar_t* shortcutPath);
	HRESULT CreateToastXml(_In_ IToastNotificationManagerStatics *toastManager, _Outptr_ IXmlDocument** inputXml, _In_ wchar_t const* image, _In_ wchar_t *text[]);
public:
	HRESULT TryCreateShortcut();
	HRESULT DisplayToast(wchar_t const *image, wchar_t ** text);
};