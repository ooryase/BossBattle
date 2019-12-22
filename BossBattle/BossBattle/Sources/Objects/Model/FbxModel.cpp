#include"FbxModel.h"

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

	BufferCreate();


	// オブジェクトの反映
	/*UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &verBuffer, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->IASetInputLayout(pVertexLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	//pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
	pDeviceContext->GSSetConstantBuffers(0, 1, &pConstantBuffer);
	pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
	pDeviceContext->GSSetShader(pGeometryShader, NULL, 0);
	pDeviceContext->RSSetState(pRasterizerState);
	*/

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