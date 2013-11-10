/*
 * Config.h
 *
 *  Created on: Dec 20, 2010
 *      Author: fmaker
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include <string>
using namespace std;


typedef struct{
	string radio;
	string exposure_mode;
	string flash_mode;
	string color_mode;
	string width;
	string whitebalance_mode;
	string zoom;
}cparams_t;

typedef enum{
	RADIO,
	EXPOSURE_MODE,
	FLASH_MODE,
	COLOR_MODE,
	WIDTH, // Height always matches
	WHITEBALANCE_MODE,
	ZOOM,
	NUM_CONFIG_PARAMS
}config_param;

// Popular vector list of parameters in device db
static const string colNames[] = \
	{"radio","exposure_mode","flash_mode","color_mode","width","whitebalance_mode","zoom"};

// Doe Categories
static const config_param quant_params[]={RADIO,FLASH_MODE,COLOR_MODE,WIDTH,ZOOM};
static const config_param qual_params[]={EXPOSURE_MODE,WHITEBALANCE_MODE};

// Full Parameter values
static const string radio_array[]={"bluetooth","wifi"};
static const string exposure_mode_array[]={"auto", "center", "backlight", "night"};
static const string flash_mode_array[]={"none","auto","red_eye_reduce","forced"};
static const string color_mode_array[]={"JPEG_Exif","RGB12","RGB16","RGB"};
static const string width_array[]={"640","800","1280"};
static const string whitebalance_mode_array[]={"daylight","fluorescent","tungsten","auto","cloudy"};
static const string zoom_array[]={"0","8","16","24","32","40","48","56","64","72"};

// Convience Macros
#define	max_str(array) (array[sizeof(array)/sizeof(string)-1])
#define	min_str(array) (array[0])
#define size_str(array)    (sizeof(array)/sizeof(string))


class Config {
public:
	Config();
	virtual ~Config();
	void reinit();
	double getEnergy() const;
    string getColor_mode() const;
    string getExposure_mode() const;
    string getFlash_mode() const;
    double getPower() const;
    string getWhitebalance_mode() const;
    string getWidth() const;
    void setColor_mode(string color_mode);
    void setExposure_mode(string exposure_mode);
    void setFlash_mode(string flash_mode);
    void setPower(double power);
    void setWhitebalance_mode(string whitebalance_mode);
    void setWidth(string width);
    int getConfigNum() const;
    string getHeight() const;
    string getZoom() const;
    void setConfig_num(int config_num);
    void setEnergy(double energy);
    void setHeight(string height);
    void setZoom(string zoom);
    double getDuration() const;
    void setDuration(double duration);
    double getTotalEnergy() const;
    void setTotalEnergy(double totalEnergy);
    double getQuality() const;
    void setQuality(double quality);
    string getRadio() const;
    void setRadio(string radio);
    string toString() const;
private:
    int config_num;
	double power;
	double energy;
	double quality;
	double duration;
	string width;
	string height;
	string zoom;
	string exposure_mode;
	string flash_mode;
	string color_mode;
	string whitebalance_mode;
	string radio;

};

ostream& operator<<(ostream& out, const Config& c);

#endif /* CONFIG_H_ */
