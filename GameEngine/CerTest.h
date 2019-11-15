//#pragma once
//#include "cereal/cereal.hpp"
//#include "cereal/archives/json.hpp"
//#include <fstream>
//struct VECTOR3
//{
//	float x, y, z;
//	template<class Archive>
//	void serialize(Archive& archive)
//	{
//		archive(CEREAL_NVP(x),CEREAL_NVP(y),CEREAL_NVP(z));
//	}
//};
//
//
//{
//	VECTOR3 vector;
//	vector.x = 1.0f;
//	vector.y = 1.0f;
//	vector.z = 1.0f;
//}
////Out
//{
//	std::ofstream stream("~");
//cereal::JSONOutputArchive outArchive(stream);
//outArchive(vector); 
//}
////Input
//{
//	std::ifstream stream("~");
//	cereal::JSONInputArchive outArchive(stream);
//	inArchive(vector);
//}

//class CPlayer
//{
//	VECTOR3 m_position;
//	int m_Life;
//	template<--->
//		void serialize(----)
//	{
//		archivel(CERAL_NVP(m_position),
//			CEREAL_NVP(m_Life));
//	}
//};