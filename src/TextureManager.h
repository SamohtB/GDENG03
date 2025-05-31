//#pragma once
//#include <vector>
//#include <memory>
//#include <unordered_map>
//
//class Texture;
//
//class TextureManager
//{
//public:
//    TextureManager();
//    ~TextureManager() = default;
//
//    enum TextureType : int
//    {
//        UNSET = -1,
//        CRATE = 0
//    };
//
//    using TexturePtr = std::shared_ptr<Texture>;
//    using String = std::string
//    using TextureList = std::vector<TexturePtr>;
//    using SRVMap = std::unordered_map<TextureType, UINT>;
//
//    void LoadTexture(const TextureType& type, const std::wstring& filepath);
//
//private:
//    SRVMap m_srvMap;
//    TextureList m_textureList; // just for storing
//};