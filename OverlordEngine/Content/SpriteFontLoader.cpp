#include "stdafx.h"
#include "SpriteFontLoader.h"

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);
	Logger::LogInfo(L"{} {}", loadInfo.assetFullPath.c_str(), loadInfo.assetSubPath.c_str());
	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	//TODO_W5(L"Implement SpriteFontLoader >> Parse .fnt file")
	//See BMFont Documentation for Binary Layout

	//Parse the Identification bytes (B,M,F)
	//If Identification bytes doesn't match B|M|F,
	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	if (pReader->Read<char>() != 'B')
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font");
		return nullptr;
	}
	if (pReader->Read<char>() != 'M')
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font");
		return nullptr;
	}
	if (pReader->Read<char>() != 'F')
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font");
		return nullptr;
	}


	//Parse the version (version 3 required)
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	if (pReader->Read<char>() != 3)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported");
		return nullptr;
	}

	//Valid .fnt file >> Start Parsing!
	//use this SpriteFontDesc to store all relevant information (used to initialize a SpriteFont object)
	SpriteFontDesc fontDesc{};

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve the FontSize [fontDesc.fontSize]
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	//Retrieve the FontName [fontDesc.fontName]
	//...
	char blockId = pReader->Read<char>();
	int blockSize = pReader->Read<int>();
	fontDesc.fontSize = pReader->Read<short int>();
	pReader->MoveBufferPosition(12);
	fontDesc.fontName = pReader->ReadNullString();

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]
	//Retrieve PageCount
	//> if pagecount > 1
	//	> Log Error (Only one texture per font is allowed!)
	//Advance to Block2 (Move Reader)
	//...
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();
	pReader->MoveBufferPosition(4);
	fontDesc.textureWidth = pReader->Read<short int>();
	fontDesc.textureHeight = pReader->Read<short int>();
	short int pages{ pReader->Read<short int>() };
	if (pages > 1)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed");
		return nullptr;
	}
	pReader->MoveBufferPosition(5);

	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve the PageName (BinaryReader::ReadNullString)
	//Construct the full path to the page texture file
	//	>> page texture should be stored next to the .fnt file, pageName contains the name of the texture file
	//	>> full texture path = asset parent_path of .fnt file (see loadInfo.assetFullPath > get parent_path) + pageName (filesystem::path::append)
	//	>> Load the texture (ContentManager::Load<TextureData>) & Store [fontDesc.pTexture]
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();
	std::wstring pageName =  pReader->ReadNullString();
	if (pageName.length() == 0)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty]");
		return nullptr;
	}
	std::wstring folderPath = loadInfo.assetSubPath.substr(0, loadInfo.assetSubPath.find(L"/") + 1);
	fontDesc.pTexture = ContentManager::Load<TextureData>(folderPath + pageName);


	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve Character Count (see documentation)
	//Create loop for Character Count, and:
	//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
	//> Create instance of FontMetric (struct)
	//	> Set Character (CharacterId) [FontMetric::character]
	//	> Retrieve Xposition (store Local)
	//	> Retrieve Yposition (store Local)
	//	> Retrieve & Set Width [FontMetric::width]
	//	> Retrieve & Set Height [FontMetric::height]
	//	> Retrieve & Set OffsetX [FontMetric::offsetX]
	//	> Retrieve & Set OffsetY [FontMetric::offsetY]
	//	> Retrieve & Set AdvanceX [FontMetric::advanceX]
	//	> Retrieve & Set Page [FontMetric::page]
	//	> Retrieve Channel (BITFIELD!!!) 
	//		> See documentation for BitField meaning [FontMetrix::channel]
	//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
	//
	//> Insert new FontMetric to the metrics [font.metrics] map
	//	> key = (wchar_t) charId
	//	> value = new FontMetric
	//(loop restarts till all metrics are parsed)
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();

	int count = blockSize / 20;
	for (int i{ 0 }; i < count; ++i)
	{
		wchar_t characterId{ wchar_t(pReader->Read<int>()) };
		if (!(characterId))
		{
			pReader->MoveBufferPosition(16);
			Logger::LogWarning(L"SpriteFontLoader::LoadContent > Character not valid");
			continue;
		}
		
		FontMetric metric{ fontDesc.metrics[characterId]};
		metric.character = characterId;
		short int xPos{ pReader->Read<short int>() };
		short int yPos{ pReader->Read<short int>() };
		metric.width = pReader->Read<unsigned short int>();
		metric.height = pReader->Read<unsigned short int>();
		metric.offsetX = pReader->Read<short int>();
		metric.offsetY = pReader->Read<short int>();
		metric.advanceX = pReader->Read<short int>();
		metric.page = pReader->Read<unsigned char>();
		unsigned char channel{ pReader->Read<unsigned char>() };
		if (channel == 1) metric.channel = 2;
		else if (channel == 2) metric.channel = 1;
		else if (channel == 4) metric.channel = 0;
		else if (channel == 8) metric.channel = 3;
		DirectX::XMFLOAT2 tex{ fontDesc.pTexture->GetDimension() };
		metric.texCoord = DirectX::XMFLOAT2{ xPos / tex.x, yPos / tex.y };
	
		fontDesc.metrics.emplace(characterId, metric);
	}
	
	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
