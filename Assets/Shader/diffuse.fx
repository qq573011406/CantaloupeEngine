//--------------------------------------------------------------------------------------
// File: EffectParam.fx
//
// The effect file for the EffectParam sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
shared float4x4 g_mWorld;                     // World view matrix
shared float4x4 g_mView; // World * View * Projection matrix
shared float4x4 g_mProj; // World * View * Projection matrix


struct VS_INPUT
{
    float4 position:POSITION;
    float4 color:COLOR0;  
};

struct VS_OUTPUT
{
    float4 position:POSITION;
};

VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    //output.position = mul(input.position,g_mWorld);
    //output.position = mul(output.position,g_mView);
    //output.position = mul(output.position,g_mProj);
    output.position = input.position;
    return output;
}

struct PS_INPUT
{
    float4 color:COLOR0;
};
struct PS_OUTPUT
{
    float4 color:COLOR0;
};

PS_OUTPUT PS_Main(PS_INPUT input)
{

    PS_OUTPUT output = (PS_OUTPUT)0;
    output.color = float4(1,1,1,1);
    return output;
}


//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique Default
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS_Main();
        PixelShader  = compile ps_2_0 PS_Main();
    }
}
