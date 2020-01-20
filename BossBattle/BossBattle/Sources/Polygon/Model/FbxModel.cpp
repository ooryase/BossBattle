#include"FbxModel.h"

using namespace MODEL;

FbxModel::FbxModel(ID3D11Device* pDevice, const char fileName[]) : BaseModel()
{
	//FBXの読み込み
	fbxManager = FbxManager::Create();
	fbxScene = FbxScene::Create(fbxManager, "fbxScene");
	FbxString FileName(fileName);
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "imp");
	fbxImporter->Initialize(FileName.Buffer(), -1, fbxManager->GetIOSettings());
	fbxImporter->Import(fbxScene);
	fbxImporter->Destroy();

	FbxLoad();

	CreateBuffer(pDevice);
}

FbxModel::~FbxModel()
{
	fbxScene->Destroy();
	fbxManager->Destroy();
}

void FbxModel::FbxLoad()
{
	

	//アニメーションデータの取り出し
	//FbxArray<FbxString*> AnimStackNameArray;
	fbxScene->FillAnimStackNameArray(AnimStackNameArray);

	//再生するアニメーションの設定
	AnimStackNumber = 0;
	FbxAnimStack* AnimationStack = fbxScene->FindMember<FbxAnimStack>(AnimStackNameArray[AnimStackNumber]->Buffer());
	fbxScene->SetCurrentAnimationStack(AnimationStack);

	FbxTakeInfo* takeInfo = fbxScene->GetTakeInfo(*(AnimStackNameArray[AnimStackNumber]));
	start = takeInfo->mLocalTimeSpan.GetStart();
	stop = takeInfo->mLocalTimeSpan.GetStop();
	FrameTime.SetTime(0, 0, 0, 1, 0, fbxScene->GetGlobalSettings().GetTimeMode());
	timeCount = start;

	// メッシュデータの取り出し
	for (int i = 0; i < fbxScene->GetRootNode()->GetChildCount(); i++) {
		if (fbxScene->GetRootNode()->GetChild(i)->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
			fbxMeshNode = fbxScene->GetRootNode()->GetChild(i);
			fbxMesh = fbxMeshNode->GetMesh();
			break;
		}
	}

	// 頂点データの取り出し
	int* index = fbxMesh->GetPolygonVertices();
	indices = std::vector<UINT>(index, index + fbxMesh->GetPolygonVertexCount());
	//vertices = new VERTEX[fbxMesh->GetControlPointsCount()];

	for (int i = 0; i < fbxMesh->GetControlPointsCount(); i++) {

		vertices.push_back(VERTEX(
			DirectX::XMFLOAT3(
			(FLOAT)fbxMesh->GetControlPointAt(i)[0],
				(FLOAT)fbxMesh->GetControlPointAt(i)[1],
				(FLOAT)fbxMesh->GetControlPointAt(i)[2]),
			DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)
		));
	}

	//法線データの取り出し
	FbxGeometryElementNormal* normalElement = fbxMesh->GetElementNormal();
	if (!normalElement)
	{
		std::cout << "Error Normal Read" << std::endl;
	}

	if (normalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		for (int vertexIndex = 0; vertexIndex < fbxMesh->GetControlPointsCount(); vertexIndex++)
		{
			int normalIndex = 0;
			if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
				normalIndex = vertexIndex;

			else if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
				normalIndex = normalElement->GetIndexArray().GetAt(vertexIndex);

			FbxVector4 normal = normalElement->GetDirectArray().GetAt(normalIndex);

			vertices.at(vertexIndex).Nor.x = (FLOAT)normal[0];
			vertices.at(vertexIndex).Nor.y = (FLOAT)normal[1];
			vertices.at(vertexIndex).Nor.z = (FLOAT)normal[2];

		}
	}
	else if (normalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		int c = fbxMesh->GetControlPointsCount();
		//fbxMesh->Get

		int lIndexByPolygonVertex = 0;
		int polygonCount = fbxMesh->GetPolygonCount();

		//Let's get normals of each polygon, since the mapping mode of normal element is by polygon-vertex.
		for (int lPolygonIndex = 0; lPolygonIndex < polygonCount; lPolygonIndex++)
		{

			//get polygon size, you know how many vertices in current polygon.
			int lPolygonSize = fbxMesh->GetPolygonSize(lPolygonIndex);
			//retrieve each vertex of current polygon.
			for (int i = 0; i < lPolygonSize; i++)
			{
				int normalIndex = 0;
				//reference mode is direct, the normal index is same as lIndexByPolygonVertex.
				if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
					normalIndex = lIndexByPolygonVertex;

				//reference mode is index-to-direct, get normals by the index-to-direct
				if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					normalIndex = normalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);

				//Got normals of each polygon-vertex.
				FbxVector4 lNormal = normalElement->GetDirectArray().GetAt(normalIndex);

				int vertexPolygon = indices.at(lIndexByPolygonVertex); //*index;
				vertices.at(vertexPolygon).Nor.x = (FLOAT)lNormal[0];
				vertices.at(vertexPolygon).Nor.y = (FLOAT)lNormal[1];
				vertices.at(vertexPolygon).Nor.z = (FLOAT)lNormal[2];

				//index++;
				lIndexByPolygonVertex++;
			}//end for i //lPolygonSize
		}//end for lPolygonIndex //PolygonCount
	}

	//隣接付線の作成
	int polygonCount = fbxMesh->GetPolygonCount();
	//int setpos = 0;
	for (int i = 0; i < polygonCount; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int a = i + 1; a < polygonCount; a++)
			{

				for (int b = 0; b < 3; b++)
				{
					if (indices.at(i * 3 + j) == indices.at(a * 3 + b) &&
						indices.at(i * 3 + (j + 1) % 3) == indices.at(a * 3 + (b + 2) % 3))
					{
	
						lineAdj.push_back(indices.at(i * 3 + (j + 2) % 3));
						lineAdj.push_back(indices.at(i * 3 + j));
						lineAdj.push_back(indices.at(i * 3 + (j + 1) % 3));
						lineAdj.push_back(indices.at(a * 3 + (b + 1) % 3));

						goto nextCheck;
					}
				}
			}
		nextCheck:;
		}
	}
	
}

void FbxModel::CreateVertexBuffer(ID3D11Device* pDevice)
{
	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertices.size();
	bd_vertex.Usage = D3D11_USAGE_DYNAMIC;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices.data();
	pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVerBuffer);

}

void FbxModel::CreateIndexBuffer(ID3D11Device* pDevice)
{
	// インデックスデータの取り出しとバッファの設定
	D3D11_BUFFER_DESC bd_index;
	bd_index.ByteWidth = sizeof(UINT) * indices.size();
	bd_index.Usage = D3D11_USAGE_DEFAULT;
	bd_index.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd_index.CPUAccessFlags = 0;
	bd_index.MiscFlags = 0;
	bd_index.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_index;
	data_index.pSysMem = indices.data();
	pDevice->CreateBuffer(&bd_index, &data_index, &pIndexBuffer);

	// インデックスデータの取り出しとバッファの設定
	D3D11_BUFFER_DESC bd_lineAdj_index;
	bd_lineAdj_index.ByteWidth = sizeof(UINT) * lineAdj.size();
	bd_lineAdj_index.Usage = D3D11_USAGE_DEFAULT;
	bd_lineAdj_index.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd_lineAdj_index.CPUAccessFlags = 0;
	bd_lineAdj_index.MiscFlags = 0;
	bd_lineAdj_index.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_lineAdj_index;
	data_lineAdj_index.pSysMem = lineAdj.data();
	pDevice->CreateBuffer(&bd_lineAdj_index, &data_lineAdj_index, &pLineAdjIndexBuffer);
}

void FbxModel::CreateRasterizeState(ID3D11Device* pDevice)
{
	// ラスタライザの設定
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.CullMode = D3D11_CULL_FRONT;
	rdc.FrontCounterClockwise = TRUE;
	rdc.DepthBias = 0;
	rdc.DepthBiasClamp = 0;
	rdc.SlopeScaledDepthBias = 0;
	rdc.DepthClipEnable = TRUE;
	rdc.ScissorEnable = FALSE;
	rdc.MultisampleEnable = FALSE;
	rdc.AntialiasedLineEnable = FALSE;
	pDevice->CreateRasterizerState(&rdc, &pRasterizerState);

}

void FbxModel::SetAnimSackNumber(int num)
{
	//再生するアニメーションの設定
	AnimStackNumber = num;
	FbxAnimStack* AnimationStack = fbxScene->FindMember<FbxAnimStack>(AnimStackNameArray[AnimStackNumber]->Buffer());
	fbxScene->SetCurrentAnimationStack(AnimationStack);

	FbxTakeInfo* takeInfo = fbxScene->GetTakeInfo(*(AnimStackNameArray[AnimStackNumber]));
	start = takeInfo->mLocalTimeSpan.GetStart();
	stop = takeInfo->mLocalTimeSpan.GetStop();
	FrameTime.SetTime(0, 0, 0, 1, 0, fbxScene->GetGlobalSettings().GetTimeMode());
	timeCount = start;

}

void FbxModel::Update()
{

	//再生するアニメーションの設定
	//AnimStackNumber = 0;
	//FbxAnimStack* AnimationStack = fbxScene->FindMember<FbxAnimStack>(AnimStackNameArray[AnimStackNumber]->Buffer());
	//fbxScene->SetCurrentAnimationStack(AnimationStack);
	//
	//FbxTakeInfo* takeInfo = fbxScene->GetTakeInfo(*(AnimStackNameArray[AnimStackNumber]));
	//start = takeInfo->mLocalTimeSpan.GetStart();
	//stop = takeInfo->mLocalTimeSpan.GetStop();
	//FrameTime.SetTime(0, 0, 0, 1, 0, fbxScene->GetGlobalSettings().GetTimeMode());
	//timeCount = start;

	if (!anim)
		return;

	timeCount += FrameTime / 8;
	if (timeCount > stop) timeCount = start;

	//アニメーション処理
	FbxMatrix globalPosition = fbxMeshNode->EvaluateGlobalTransform(timeCount);
	FbxVector4 t0 = fbxMeshNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 r0 = fbxMeshNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 s0 = fbxMeshNode->GetGeometricScaling(FbxNode::eSourcePivot);
	FbxAMatrix geometryOffset = FbxAMatrix(t0, r0, s0);

	//ボーン計算
	FbxMatrix* clusterDeformation = new FbxMatrix[fbxMesh->GetControlPointsCount()];
	memset(clusterDeformation, 0, sizeof(FbxMatrix) * fbxMesh->GetControlPointsCount());

	FbxSkin* skinDeformer = (FbxSkin*)fbxMesh->GetDeformer(0, FbxDeformer::eSkin);
	int clusterCount = skinDeformer->GetClusterCount();
	for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++)
	{
		FbxCluster* cluster = skinDeformer->GetCluster(clusterIndex);
		FbxMatrix vertexTransformMatrix;
		FbxAMatrix referenceGlobalInitPosition;
		FbxAMatrix clusterGlobalInitPosition;
		FbxMatrix clusterGlobalCurrentPosition;
		FbxMatrix clusterRelativeInitPosition;
		FbxMatrix clusterRelativeCurrentPositionInverse;
		cluster->GetTransformMatrix(referenceGlobalInitPosition);
		referenceGlobalInitPosition *= geometryOffset;
		cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);
		clusterGlobalCurrentPosition = cluster->GetLink()->EvaluateGlobalTransform(timeCount);
		clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;
		clusterRelativeCurrentPositionInverse = globalPosition.Inverse() * clusterGlobalCurrentPosition;
		vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;
		//clusterDeformationに各頂点毎の影響度に応じてそれぞれに加算
		for (int i = 0; i < cluster->GetControlPointIndicesCount(); i++)
		{
			int index = cluster->GetControlPointIndices()[i];
			double weight = cluster->GetControlPointWeights()[i];
			FbxMatrix influence = vertexTransformMatrix * weight;
			clusterDeformation[index] += influence;
		}
	}

	//最終的な頂点座標を計算しVERTEXに変換
	for (int i = 0; i < fbxMesh->GetControlPointsCount(); i++)
	{
		FbxVector4 outVertex = clusterDeformation[i].MultNormalize(fbxMesh->GetControlPointAt(i));
		vertices.at(i).Pos.x = (FLOAT)outVertex[0];
		vertices.at(i).Pos.y = (FLOAT)outVertex[1];
		vertices.at(i).Pos.z = (FLOAT)outVertex[2];

		//FbxVector4 outVertex = clusterDeformation[i].MultNormalize(fbxMesh->GetControlPointAt(i));
		//vertices[i].Pos.x = (FLOAT)outVertex[0];
		//vertices[i].Pos.y = (FLOAT)outVertex[1];
		//vertices[i].Pos.z = (FLOAT)outVertex[2];
	}

	delete[] clusterDeformation;


}