#include "BlendMode.h"

#include <cassert>

void BlendMode::Configurator::Initialize(BlendModes _mode)
{
	blendDesc_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc_.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc_.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc_.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	ApplyMode(_mode);
}

void BlendMode::Configurator::ApplyMode(BlendModes _mode)
{
	switch (_mode)
	{
	case BlendMode::BlendModes::None:
		blendDesc_.RenderTarget[0].BlendEnable = FALSE;
		break;

	case BlendMode::BlendModes::Normal:
		blendDesc_.RenderTarget[0].BlendEnable = TRUE;
		blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;

	case BlendMode::BlendModes::Add:
		blendDesc_.RenderTarget[0].BlendEnable = TRUE;
		blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendMode::BlendModes::Subtract:
		blendDesc_.RenderTarget[0].BlendEnable = TRUE;
		blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendMode::BlendModes::Multiply:
		blendDesc_.RenderTarget[0].BlendEnable = TRUE;
		blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
		blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
		break;

	case BlendMode::BlendModes::Screen:
		blendDesc_.RenderTarget[0].BlendEnable = TRUE;
		blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendMode::BlendModes::COUNT:
		assert(0 && "BlendModeにCOUNTが指定されています。");
		break;
	}

	return;
}
