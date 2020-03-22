#version 420

//uniform mat4 MVP;
uniform mat4 matModel;		// Model or World 
uniform mat4 matModelInverseTranspose;		// For normal calculation
uniform mat4 matView; 		// View or camera
uniform mat4 matProj;		// Projection transform

in vec4 vColour;				// Was vec3
in vec4 vPosition;				// Was vec3
in vec4 vNormal;				// Vertex normal
in vec4 vUVx2;					// 2 x Texture coords
in vec4 vTangent;				// For bump mapping
in vec4 vBiNormal;				// For bump mapping
in vec4 vBoneID;				// For skinned mesh (FBX)
in vec4 vBoneWeight;			// For skinned mesh (FBX)

//out vec3 color;
//out vec4 vertWorld;			// Location of the vertex in the world
out vec4 fColour;	
out vec4 fVertWorldLocation;
out vec4 fNormal;
out vec4 fUVx2;

const int MAXNUMBEROFBONES = 100;
uniform mat4 matBonesArray[MAXNUMBEROFBONES];
uniform bool isSkinnedMesh;

void main()
{
    vec4 vertPosition = vPosition;
	if (isSkinnedMesh)
	{
	
		mat4 BoneTransform = matBonesArray[ int(vBoneID[0]) ] * vBoneWeight[0];
				 BoneTransform += matBonesArray[ int(vBoneID[1]) ] * vBoneWeight[1];
				 BoneTransform += matBonesArray[ int(vBoneID[2]) ] * vBoneWeight[2];
				 BoneTransform += matBonesArray[ int(vBoneID[3]) ] * vBoneWeight[3];

		// Apply the bone transform to the vertex:
		vec4 vertOriginal = vec4(vertPosition.xyz, 1.0f);

		vec4 vertAfterBoneTransform = BoneTransform * vertOriginal;
				
		mat4 matMVP = matProj * matView * matModel;		
		// Transform the updated vertex location (from the bone)
		//  and transform with model view projection matrix (as usual)
		gl_Position = matMVP * vertAfterBoneTransform;
		
		
		// Then we do the normals, etc.
		fVertWorldLocation = matModel * vertAfterBoneTransform;	
		
		// Updated "world" or "model" transform 
		mat4 matModelAndBone = matModel * BoneTransform;
		
		vec3 theNormal = normalize(vNormal.xyz);
		fNormal = inverse(transpose(matModelAndBone)) * vec4(theNormal, 1.0f);
		fNormal.xyz = -normalize(fNormal.xyz);
		
		return;
	}
	
    mat4 matMVP = matProj * matView * matModel;
	
	gl_Position = matMVP * vec4(vertPosition.xyz, 1.0);
	
	// Vertex location in "world space"
	// Vec4 = mat4x4 * vec4
	fVertWorldLocation = matModel * vec4(vertPosition.xyz, 1.0);		
	
	//mat4 matModelInverseTranspose = inverse(transpose(matModel));
	
	vec3 theNormal = normalize(vNormal.xyz);
 	fNormal = matModelInverseTranspose * vec4(theNormal, 1.0f);
	fNormal.xyz = normalize(fNormal.xyz); 
	
	// Pass the colour and UV unchanged.
    fColour = vColour;	
	fUVx2 = vUVx2;
}
