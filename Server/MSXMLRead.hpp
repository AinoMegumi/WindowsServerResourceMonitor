﻿#pragma once
#include "XmlDomElement.hpp"
#include "XmlDataManager.hpp"
#include <unordered_map>

namespace MSXML {
	class Read : public std::unordered_map<std::wstring, XmlDataManager::wstring> {
	private:
		using Base = std::unordered_map<std::wstring, XmlDataManager::wstring>;
		XmlDomDocument lpXmlDoc;
		XmlDataManager::wstring GetW(const std::wstring& Path);
	public:
		Read(const std::wstring& FilePath);
		
		template<typename T, std::enable_if_t<std::is_same_v<T, std::wstring>, std::nullptr_t> = nullptr>
		XmlDataManager::wstring Get(const std::wstring& Path) { return this->GetW(Path); }

		template<typename T, std::enable_if_t<std::is_same_v<T, std::string>, std::nullptr_t> = nullptr>
		XmlDataManager::string Get(const std::wstring& Path) { return XmlDataManager::string(this->GetW(Path)); }

		//template<typename T, std::enable_if_t<std::is_signed_v<T>&& std::is_integral_v<T>, std::nullptr_t> = nullptr>
		//XmlDataManager::number<T> Get(const std::wstring& Path) { return XmlDataManager::number<T>(this->GetW(Path)); }
		//
		//template<typename T, std::enable_if_t<std::is_unsigned_v<T>&& std::is_integral_v<T>, std::nullptr_t> = nullptr>
		//XmlDataManager::number<T> Get(const std::wstring& Path) { return XmlDataManager::number<T>(this->GetW(Path)); }
		//
		//template<typename T, std::enable_if_t<std::is_floating_point_v<T>, std::nullptr_t> = nullptr>
		//XmlDataManager::number<T> Get(const std::wstring& Path) { return XmlDataManager::number(this->GetW(Path)); }

		template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, std::nullptr_t> = nullptr>
		XmlDataManager::number<T> Get(const std::wstring& Path) { return XmlDataManager::number(GetW(Path)); }
	};
}
