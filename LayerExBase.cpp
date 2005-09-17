#include "LayerExBase.h"

/// �v���p�e�B���� int �l���擾����
static tjs_int64 getPropValue(iTJSDispatch2 *dispatch, iTJSDispatch2 *layerobj)
{
	tTJSVariant var;
	if(TJS_FAILED(dispatch->PropGet(0, NULL, NULL, &var, layerobj))) {
		TVPThrowExceptionMessage(TJS_W("can't get int value from property."));
	}
	return var;
}
	
void
NI_LayerExBase::reset(iTJSDispatch2 *layerobj)
{
	// ���C������摜�o�b�t�@�����擾����
	tjs_uint8 * buffer;
	tjs_int pitch;
	tjs_int width;
	tjs_int height;
	
	// ���擾
	width  = (int)getPropValue(_widthProp, layerobj);
	height = (int)getPropValue(_heightProp, layerobj);
	buffer = (tjs_uint8 *)getPropValue(_bufferProp, layerobj);
	pitch  = (int)getPropValue(_pitchProp, layerobj);
	
	// �ύX����ĂȂ��ꍇ�͂���Ȃ���
	if (!(width == _width &&
		  height == _height &&
		  pitch == _pitch &&
		  buffer == _buffer)) {
		_width = width;
		_height = height;
		_pitch = pitch;
		_buffer = buffer;
	}
}

/**
 * �X�V�����Ăяo��
 * layer.update(x,y,w,h) ���Ăяo��
 * �摜�̈�S�̂��X�V�Ƃ���
 */
void
NI_LayerExBase::redraw(iTJSDispatch2 *layerobj)
{
	tTJSVariant vars[4];
	_leftProp->PropGet(0,NULL,NULL,&vars[0], layerobj);
	_topProp->PropGet(0,NULL,NULL,&vars[1], layerobj);
	_widthProp->PropGet(0,NULL,NULL,&vars[2], layerobj);
	_heightProp->PropGet(0,NULL,NULL,&vars[3], layerobj);
	tTJSVariant *varsp[4] = {vars, vars+1, vars+2, vars+3};
	_updateProp->FuncCall(0, NULL, NULL, NULL, 4, varsp, layerobj);
}

NI_LayerExBase::NI_LayerExBase(iTJSDispatch2 *layerobj)
{
	// �v���p�e�B�擾
	tTJSVariant var;

	if (TJS_FAILED(layerobj->PropGet(TJS_IGNOREPROP, TJS_W("imageLeft"), NULL, &var, layerobj))) {
		TVPThrowExceptionMessage(TJS_W("invoking of Layer.imageLeft failed."));
	} else {
		_leftProp = var;
	}
	if (TJS_FAILED(layerobj->PropGet(TJS_IGNOREPROP, TJS_W("imageTop"), NULL, &var, layerobj))) {
		TVPThrowExceptionMessage(TJS_W("invoking of Layer.imageTop failed."));
	} else {
		_topProp = var;
	}
	if (TJS_FAILED(layerobj->PropGet(TJS_IGNOREPROP, TJS_W("imageWidth"), NULL, &var, layerobj))) {
		TVPThrowExceptionMessage(TJS_W("invoking of Layer.imageWidth failed."));
	} else {
		_widthProp = var;
	}
	if (TJS_FAILED(layerobj->PropGet(TJS_IGNOREPROP, TJS_W("imageHeight"), NULL, &var, layerobj))) {
		TVPThrowExceptionMessage(TJS_W("invoking of Layer.imageHeight failed."));
	} else {
		_heightProp = var;
	}
	if (TJS_FAILED(layerobj->PropGet(TJS_IGNOREPROP, TJS_W("mainImageBufferForWrite"), NULL, &var, layerobj))) {
		TVPThrowExceptionMessage(TJS_W("invoking of Layer.mainImageBufferForWrite failed."));
	} else {
		_bufferProp = var;
	}
	if (TJS_FAILED(layerobj->PropGet(TJS_IGNOREPROP, TJS_W("mainImageBufferPitch"), NULL, &var, layerobj))) {
		TVPThrowExceptionMessage(TJS_W("invoking of Layer.mainImageBufferPitch failed."));
	} else {
		_pitchProp = var;
	}
	if (TJS_FAILED(layerobj->PropGet(TJS_IGNOREPROP, TJS_W("update"), NULL, &var, layerobj))) {
		TVPThrowExceptionMessage(TJS_W("invoking of Layer.update failed."));
	} else {
		_updateProp = var;
	}
	
	_width = 0;
	_height = 0;
	_pitch = 0;
	_buffer = NULL;
}

/**
 * �f�X�g���N�^
 */
NI_LayerExBase::~NI_LayerExBase()
{
	_leftProp->Release();
	_topProp->Release();
	_widthProp->Release();
	_heightProp->Release();
	_bufferProp->Release();
	_pitchProp->Release();
	_updateProp->Release();
}
