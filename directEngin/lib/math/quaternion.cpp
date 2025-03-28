#include "pch.h"
#include "quaternion.h"


const Quaternion Quaternion::s_qepsilon(DirectX::XMVectorReplicate(Vector3::s_epsilon));
const Quaternion Quaternion::identity{};