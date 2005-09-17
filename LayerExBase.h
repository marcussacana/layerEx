#ifndef __LayerExBase__
#define __LayerExBase__

#include <windows.h>
#include "tp_stub.h"

/**
 * ���C���g�� ��{���ێ��p�l�C�e�B�u�C���X�^���X�B
 */
class NI_LayerExBase : public tTJSNativeInstance
{
protected:
	// ���C����������擾���邽�߂̃v���p�e�B
	// �����ł����������邽�߃L���b�V�����Ă���
	iTJSDispatch2 * _leftProp;
	iTJSDispatch2 * _topProp;
	iTJSDispatch2 * _widthProp;
	iTJSDispatch2 * _heightProp;
	iTJSDispatch2 * _pitchProp;
	iTJSDispatch2 * _bufferProp;
	iTJSDispatch2 * _updateProp;
	
	// ���C������r�ێ��p
	tjs_int _width;
	tjs_int _height;
	tjs_int _pitch;
	tjs_uint8* _buffer;

public:
	/**
	 * �ĕ`��v��
	 */
	virtual void redraw(iTJSDispatch2 *layerobj);
	
	/**
	 * �O���t�B�b�N������������
	 * ���C���̃r�b�g�}�b�v��񂪕ύX����Ă���\��������̂Ŗ���`�F�b�N����B
	 * �ύX����Ă���ꍇ�͕`��p�̃R���e�L�X�g��g�݂Ȃ���
	 * @param layerobj ���C���I�u�W�F�N�g
	 */
	virtual void reset(iTJSDispatch2 *layerobj);
	
public:
	/**
	 * �R���X�g���N�^
	 */
	NI_LayerExBase(iTJSDispatch2 *layerobj);

	/**
	 * �f�X�g���N�^
	 */
	~NI_LayerExBase();
};

#endif
