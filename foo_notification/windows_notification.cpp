#include "stdafx.h"
#include "windows_notification.h"

const wchar_t AppId[] = L"Foobar2000";

HRESULT windows_notification::TryCreateShortcut() {
	wchar_t shortcutPath[MAX_PATH];
	DWORD charWritten = GetEnvironmentVariable(L"APPDATA", shortcutPath, MAX_PATH);
	HRESULT hr = charWritten > 0 ? S_OK : E_INVALIDARG;

	if (SUCCEEDED(hr)) {
		errno_t concatError = wcscat_s(shortcutPath, ARRAYSIZE(shortcutPath), L"\\Microsoft\\Windows\\Start Menu\\Programs\\Foobar 2000.lnk");
		hr = concatError == 0 ? S_OK : E_INVALIDARG;
		if (SUCCEEDED(hr)) {
			DWORD attributes = GetFileAttributes(shortcutPath);
			bool fileExists = attributes < 0xFFFFFFF;

			if (!fileExists) {
				hr = InstallShortcut(shortcutPath);
			} else {
				hr = S_FALSE;
			}
		}
	}
	return hr;
}

// Install the shortcut
HRESULT windows_notification::InstallShortcut(_In_z_ wchar_t *shortcutPath) {
	wchar_t exePath[MAX_PATH];

	DWORD charWritten = GetModuleFileNameEx(GetCurrentProcess(), nullptr, exePath, ARRAYSIZE(exePath));

	HRESULT hr = charWritten > 0 ? S_OK : E_FAIL;

	if (SUCCEEDED(hr)) {
		ComPtr<IShellLink> shellLink;
		hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&shellLink));

		if (SUCCEEDED(hr)) {
			hr = shellLink->SetPath(exePath);
			if (SUCCEEDED(hr)) {
				hr = shellLink->SetArguments(L"");
				if (SUCCEEDED(hr)) {
					ComPtr<IPropertyStore> propertyStore;

					hr = shellLink.As(&propertyStore);
					if (SUCCEEDED(hr)) {
						PROPVARIANT appIdPropVar;
						hr = InitPropVariantFromString(AppId, &appIdPropVar);
						if (SUCCEEDED(hr)) {
							hr = propertyStore->SetValue(PKEY_AppUserModel_ID, appIdPropVar);
							if (SUCCEEDED(hr)) {
								hr = propertyStore->Commit();
								if (SUCCEEDED(hr)) {
									ComPtr<IPersistFile> persistFile;
									hr = shellLink.As(&persistFile);
									if (SUCCEEDED(hr)) {
										hr = persistFile->Save(shortcutPath, TRUE);
									}
								}
							}
							PropVariantClear(&appIdPropVar);
						}
					}
				}
			}
		}
	}
	return hr;
}

HRESULT windows_notification::DisplayToast(wchar_t const *image, wchar_t ** text) {
	ComPtr<IToastNotificationManagerStatics> toastStatics;
	HRESULT hr = GetActivationFactory(StringReferenceWrapper(RuntimeClass_Windows_UI_Notifications_ToastNotificationManager).Get(), &toastStatics);

	if (SUCCEEDED(hr)) {
		ComPtr<IXmlDocument> toastXml;

		hr = CreateToastXml(toastStatics.Get(), &toastXml, image, text);
		if (SUCCEEDED(hr)) {
			hr = CreateToast(toastStatics.Get(), toastXml.Get());
		}
	}
	return hr;
}

// Create the toast XML from a template
HRESULT windows_notification::CreateToastXml(_In_ IToastNotificationManagerStatics *toastManager, _Outptr_ IXmlDocument** inputXml, _In_ wchar_t const* image, _In_ wchar_t *text[]) {
	// Retrieve the template XML
	HRESULT hr = toastManager->GetTemplateContent(ToastTemplateType_ToastImageAndText04, inputXml);
	if (SUCCEEDED(hr)) {
		wchar_t *imagePath = (wchar_t *)image;
		hr = imagePath != nullptr ? S_OK : HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
		if (SUCCEEDED(hr)) {
			hr = SetImageSrc(imagePath, *inputXml);
			if (SUCCEEDED(hr)) {
				UINT32 textLengths[] = { wcslen(text[0]), wcslen(text[1]), wcslen(text[2]) };
				hr = SetTextValues(text, 3, textLengths, *inputXml);
			}
		}
	}
	return hr;
}

// Set the value of the "src" attribute of the "image" node
HRESULT windows_notification::SetImageSrc(_In_z_ wchar_t *imagePath, _In_ IXmlDocument *toastXml) {
	wchar_t imageSrc[MAX_PATH] = L"";
	HRESULT hr = StringCchCat(imageSrc, ARRAYSIZE(imageSrc), imagePath);
	//console::formatter() << imageSrc;
	if (SUCCEEDED(hr)) {
		ComPtr<IXmlNodeList> nodeList;
		hr = toastXml->GetElementsByTagName(StringReferenceWrapper(L"image").Get(), &nodeList);
		if (SUCCEEDED(hr)) {
			ComPtr<IXmlNode> imageNode;
			hr = nodeList->Item(0, &imageNode);
			if (SUCCEEDED(hr)) {
				ComPtr<IXmlNamedNodeMap> attributes;

				hr = imageNode->get_Attributes(&attributes);
				if (SUCCEEDED(hr)) {
					ComPtr<IXmlNode> srcAttribute;

					hr = attributes->GetNamedItem(StringReferenceWrapper(L"src").Get(), &srcAttribute);
					if (SUCCEEDED(hr)) {
						hr = SetNodeValueString(StringReferenceWrapper(imageSrc).Get(), srcAttribute.Get(), toastXml);
					}
				}
			}
		}
	}
	return hr;
}

// Set the values of each of the text nodes
HRESULT windows_notification::SetTextValues(_In_reads_(textValuesCount) wchar_t **textValues, _In_ UINT32 textValuesCount, _In_reads_(textValuesCount) UINT32 *textValuesLengths, _In_ IXmlDocument *toastXml) {
	HRESULT hr = textValues != nullptr && textValuesCount > 0 ? S_OK : E_INVALIDARG;
	if (SUCCEEDED(hr)) {
		ComPtr<IXmlNodeList> nodeList;
		hr = toastXml->GetElementsByTagName(StringReferenceWrapper(L"text").Get(), &nodeList);
		if (SUCCEEDED(hr)) {
			UINT32 nodeListLength;
			hr = nodeList->get_Length(&nodeListLength);
			if (SUCCEEDED(hr)) {
				hr = textValuesCount <= nodeListLength ? S_OK : E_INVALIDARG;
				if (SUCCEEDED(hr)) {
					for (UINT32 i = 0; i < textValuesCount; i++) {
						ComPtr<IXmlNode> textNode;
						hr = nodeList->Item(i, &textNode);
						if (SUCCEEDED(hr)) {
							hr = SetNodeValueString(StringReferenceWrapper(textValues[i], textValuesLengths[i]).Get(), textNode.Get(), toastXml);
						}
					}
				}
			}
		}
	}
	return hr;
}

HRESULT windows_notification::SetNodeValueString(_In_ HSTRING inputString, _In_ IXmlNode *node, _In_ IXmlDocument *xml) {
	ComPtr<IXmlText> inputText;

	HRESULT hr = xml->CreateTextNode(inputString, &inputText);
	if (SUCCEEDED(hr)) {
		ComPtr<IXmlNode> inputTextNode;

		hr = inputText.As(&inputTextNode);
		if (SUCCEEDED(hr)) {
			ComPtr<IXmlNode> pAppendedChild;
			hr = node->AppendChild(inputTextNode.Get(), &pAppendedChild);
		}
	}

	return hr;
}

// Create and display the toast
HRESULT windows_notification::CreateToast(_In_ IToastNotificationManagerStatics *toastManager, _In_ IXmlDocument *xml) {
	ComPtr<IToastNotifier> notifier;
	HRESULT hr = toastManager->CreateToastNotifierWithId(StringReferenceWrapper(AppId).Get(), &notifier);
	if (SUCCEEDED(hr)) {
		ComPtr<IToastNotificationFactory> factory;
		hr = GetActivationFactory(StringReferenceWrapper(RuntimeClass_Windows_UI_Notifications_ToastNotification).Get(), &factory);
		if (SUCCEEDED(hr)) {
			ComPtr<IToastNotification> toast;
			hr = factory->CreateToastNotification(xml, &toast);

			if (SUCCEEDED(hr)) {
				notifier->Show(toast.Get());
			}
		}
	}
	return hr;
}