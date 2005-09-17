#ifndef __LayerExImage__
#define __LayerExImage__

#include "LayerExBase.h"

/**
 * ���C���g�� �C���[�W����p�l�C�e�B�u�C���X�^���X
 */
class NI_LayerExImage : public NI_LayerExBase
{
public:
	NI_LayerExImage(iTJSDispatch2 *layerobj) : NI_LayerExBase(layerobj) {};
	
	/**
	 * ���b�N�A�b�v�e�[�u�����f
	 * @param pLut lookup table
	 */
	void lut(BYTE* pLut);
	
	/**
	 * ���x�ƃR���g���X�g
	 * @param brightness ���x -255 �` 255, �����̏ꍇ�͈Â��Ȃ�
	 * @param contrast �R���g���X�g -100 �`100, 0 �̏ꍇ�ω����Ȃ�
	 */
	void light(int brightness, int contrast);

	/**
	 * �F���ƍʓx
	 * @param hue �F��
	 * @param sat �ʓx
	 * @param blend �u�����h 0 (���ʂȂ�) �` 1 (full effect)
	 */
	void colorize(int hue, int sat, double blend);

	/**
	 * �m�C�Y�ǉ�
	 * @param level �m�C�Y���x�� 0 (no noise) �` 255 (lot of noise).
	 */
	void noise(int level);
};

#endif