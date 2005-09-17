#include <stdio.h>
#include <windows.h>
#include "tp_stub.h"

/**
 * ���O�o�͗p
 */
static void log(const tjs_char *format, ...)
{
	va_list args;
	va_start(args, format);
	tjs_char msg[1024];
	_vsnwprintf(msg, 1024, format, args);
	TVPAddLog(msg);
	va_end(args);
}

#include "LayerExImage.h"

// �N���XID
static tjs_int32 ClassID_LayerExImage = -1;

/**
 * ���C���I�u�W�F�N�g���烌�C���g���l�C�e�B�u�C���X�^���X���擾����B
 * �l�C�e�B�u�C���X�^���X�������ĂȂ��ꍇ�͎����I�Ɋ��蓖�Ă�
 * @param objthis ���C���I�u�W�F�N�g
 * @return Layer�g���������ݗp�l�C�e�B�u�C���X�^���X�B�擾���s������ NULL
 */
NI_LayerExImage *
getLayerExImageNative(iTJSDispatch2 *layerobj)
{
	if (!layerobj) return NULL;

	NI_LayerExImage *_this;
	if (TJS_FAILED(layerobj->NativeInstanceSupport(TJS_NIS_GETINSTANCE,
												   ClassID_LayerExImage, (iTJSNativeInstance**)&_this))) {
		_this = new NI_LayerExImage(layerobj);
		if (TJS_FAILED(layerobj->NativeInstanceSupport(TJS_NIS_REGISTER,
													   ClassID_LayerExImage, (iTJSNativeInstance **)&_this))) {
			return NULL;
		}
	}
	_this->reset(layerobj);
	return _this;
}

/**
 * ���x�ƃR���g���X�g
 * @param brightness ���x -255 �` 255, �����̏ꍇ�͈Â��Ȃ�
 * @param contrast �R���g���X�g -100 �`100, 0 �̏ꍇ�ω����Ȃ�
 */
class tLightFunction : public tTJSDispatch
{
protected:
public:
	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis) {

		if (numparams < 1) return TJS_E_BADPARAMCOUNT;

		NI_LayerExImage *image;
		if ((image = getLayerExImageNative(objthis)) == NULL) return TJS_E_NATIVECLASSCRASH;
		
		image->light(*param[0], numparams > 1 ? (int)*param[1] : 0);
		image->redraw(objthis);
		
		return TJS_S_OK;
	}
};

/**
 * �F���ƍʓx
 * @param hue �F��
 * @param sat �ʓx
 * @param blend �u�����h 0 (���ʂȂ�) �` 1 (full effect)
 */
class tColorizeFunction : public tTJSDispatch
{
protected:
public:
	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis) {

		if (numparams < 2) return TJS_E_BADPARAMCOUNT;

		NI_LayerExImage *image;
		if ((image = getLayerExImageNative(objthis)) == NULL) return TJS_E_NATIVECLASSCRASH;
		
		image->colorize(*param[0], *param[1], numparams > 2 ? *param[2] : 1.0f);
		image->redraw(objthis);
		
		return TJS_S_OK;
	}
};

/**
 * �m�C�Y�ǉ�
 * @param level �m�C�Y���x�� 0 (no noise) �` 255 (lot of noise).
 */
class tNoiseFunction : public tTJSDispatch
{
protected:
public:
	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis) {

		NI_LayerExImage *image;
		if ((image = getLayerExImageNative(objthis)) == NULL) return TJS_E_NATIVECLASSCRASH;
		
		if (numparams < 1) return TJS_E_BADPARAMCOUNT;
		image->noise(*param[0]);
		image->redraw(objthis);
		
		return TJS_S_OK;
	}
};


//---------------------------------------------------------------------------
#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason,
	void* lpReserved)
{
	return 1;
}

static void
addMethod(iTJSDispatch2 *dispatch, const tjs_char *methodName, tTJSDispatch *method)
{
	tTJSVariant var = tTJSVariant(method);
	method->Release();
	dispatch->PropSet(
		TJS_MEMBERENSURE, // �����o���Ȃ������ꍇ�ɂ͍쐬����悤�ɂ���t���O
		methodName, // �����o�� ( ���Ȃ炸 TJS_W( ) �ň͂� )
		NULL, // �q���g ( �{���̓����o���̃n�b�V���l�����ANULL �ł��悢 )
		&var, // �o�^����l
		dispatch // �R���e�L�X�g
		);
}

static void
delMethod(iTJSDispatch2 *dispatch, const tjs_char *methodName)
{
	dispatch->DeleteMember(
		0, // �t���O ( 0 �ł悢 )
		methodName, // �����o��
		NULL, // �q���g
		dispatch // �R���e�L�X�g
		);
}

static void
addClass(iTJSDispatch2 *dispatch, const tjs_char *className, iTJSDispatch2 *classObj)
{
	tTJSVariant var = tTJSVariant(classObj);
	classObj->Release();
	dispatch->PropSet(
		TJS_MEMBERENSURE, // �����o���Ȃ������ꍇ�ɂ͍쐬����悤�ɂ���t���O
		className, // �����o�� ( ���Ȃ炸 TJS_W( ) �ň͂� )
		NULL, // �q���g ( �{���̓����o���̃n�b�V���l�����ANULL �ł��悢 )
		&var, // �o�^����l
		dispatch // �R���e�L�X�g
		);
}

//---------------------------------------------------------------------------
static tjs_int GlobalRefCountAtInit = 0;
extern "C" HRESULT _stdcall _export V2Link(iTVPFunctionExporter *exporter)
{
	// �X�^�u�̏�����(�K���L�q����)
	TVPInitImportStub(exporter);

	// �N���X�I�u�W�F�N�g�o�^
	ClassID_LayerExImage = TJSRegisterNativeClass(TJS_W("LayerExImage"));

	{
		// TJS �̃O���[�o���I�u�W�F�N�g���擾����
		iTJSDispatch2 * global = TVPGetScriptDispatch();
		
		// Layer �N���X�I�u�W�F�N�g���擾
		tTJSVariant varScripts;
		TVPExecuteExpression(TJS_W("Layer"), &varScripts);
		iTJSDispatch2 *dispatch = varScripts.AsObjectNoAddRef();
		if (dispatch) {
			addMethod(dispatch, L"light",  new tLightFunction());
			addMethod(dispatch, L"colorize",  new tColorizeFunction());
			addMethod(dispatch, L"noise",  new tNoiseFunction());
		}
		
		global->Release();
	}
			
	// ���̎��_�ł� TVPPluginGlobalRefCount �̒l��
	GlobalRefCountAtInit = TVPPluginGlobalRefCount;
	// �Ƃ��čT���Ă����BTVPPluginGlobalRefCount �͂��̃v���O�C������
	// �Ǘ�����Ă��� tTJSDispatch �h���I�u�W�F�N�g�̎Q�ƃJ�E���^�̑��v�ŁA
	// ������ɂ͂���Ɠ������A����������Ȃ��Ȃ��ĂȂ��ƂȂ�Ȃ��B
	// �����Ȃ��ĂȂ���΁A�ǂ����ʂ̂Ƃ���Ŋ֐��Ȃǂ��Q�Ƃ���Ă��āA
	// �v���O�C���͉���ł��Ȃ��ƌ������ƂɂȂ�B

	return S_OK;
}
//---------------------------------------------------------------------------
extern "C" HRESULT _stdcall _export V2Unlink()
{
	// �g���g��������A�v���O�C����������悤�Ƃ���Ƃ��ɌĂ΂��֐��B

	// �������炩�̏����Ńv���O�C��������ł��Ȃ��ꍇ��
	// ���̎��_�� E_FAIL ��Ԃ��悤�ɂ���B
	// �����ł́ATVPPluginGlobalRefCount �� GlobalRefCountAtInit ����
	// �傫���Ȃ��Ă���Ύ��s�Ƃ������Ƃɂ���B
	if(TVPPluginGlobalRefCount > GlobalRefCountAtInit) return E_FAIL;
		// E_FAIL ���A��ƁAPlugins.unlink ���\�b�h�͋U��Ԃ�

	// - �܂��ATJS �̃O���[�o���I�u�W�F�N�g���擾����
	iTJSDispatch2 * global = TVPGetScriptDispatch();

	// - global �� DeleteMember ���\�b�h��p���A�I�u�W�F�N�g���폜����
	if(global)
	{
		// TJS ���̂����ɉ������Ă����Ƃ��Ȃǂ�
		// global �� NULL �ɂȂ蓾��̂� global �� NULL �łȂ�
		// ���Ƃ��`�F�b�N����

		// Layer �N���X�I�u�W�F�N�g���擾
		tTJSVariant varScripts;
		TVPExecuteExpression(TJS_W("Layer"), &varScripts);
		iTJSDispatch2 *dispatch = varScripts.AsObjectNoAddRef();
		if (dispatch) {
			delMethod(dispatch, L"light");
			delMethod(dispatch, L"colorize");
			delMethod(dispatch, L"noise");
		}
		
		// - global �� Release ����
		global->Release();
	}

	// �X�^�u�̎g�p�I��(�K���L�q����)
	TVPUninitImportStub();

	return S_OK;
}
