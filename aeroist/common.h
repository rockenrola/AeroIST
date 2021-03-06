#ifndef COMMON_H
#define COMMON_H

#define AEROIST "AeroIST"

enum ThreadStatus{
    STOPPED,
    MEASURE_RUNNING,
    REFERENCE_RUNNING
};

#define TAG_NAME "name"
#define TAG_DESCRIPTION "description"
#define TAG_MEASURES_PER_ITERATION "measures_per_iteration"
#define TAG_DATA "data"
#define TAG_RAW_DATA "raw_data"
#define TAG_SETTLING_TIME "settling_time"
#define TAG_START_VALUES "start_values"
#define TAG_START "start"
#define TAG_END "end"
#define TAG_STEP "step"
#define TAG_CONTROL_TYPE "control_type"
#define TAG_ITERATIONS "iterations"
#define TAG_DATA_REFERENCE "data_reference"
#define TAG_REFERENCE "reference"
#define TAG_REFERENCE_NAME "reference_name"
#define TAG_MEASURE "measurement"
#define TAG_ITEM "item"
#define TAG_PROJECT "project"
#define TAG_OPTIONS "options"

#define SETTINGS_VIRTUAL_MEASURES "virtual_measures"
#define SETTINGS_PROJECT_FOLDER "project_folder"
#define SETTINGS_SCHEMA_FILE "schema_file"
#define SETTINGS_SCHEMA_FILE_DEFAULT "aeroist.xsd"
#define SETTINGS_SCHEMA_CONFIRM "schema_confirm"
#define SETTINGS_GUI_STATE "gui/state"
#define SETTINGS_GUI_GEOMETRY "gui/geometry"
#define SETTINGS_GUI_RP_GEOMETRY "gui/geometry_rp"
#define SETTINGS_GUI_RD_GEOMETRY "gui/geometry_rd"
#define SETTINGS_GUI_PREF_GEOMETRY "gui/geometry_pref"
#define SETTINGS_GUI_MP_GEOMETRY "gui/geometry_mp"
#define SETTINGS_GUI_MD_GEOMETRY "gui/geometry_md"
#define SETTINGS_GUI_CD_GEOMETRY "gui/geometry_cd"
#define SETTINGS_GUI_CN_GEOMETRY "gui/geometry_cn"
#define SETTINGS_GUI_CN_CHECKBOX_AVG "gui/checkbox_avg_cn"
#define SETTINGS_GUI_CN_CHECKBOX_ERR "gui/checkbox_err_cn"
#define SETTINGS_GUI_CN_LINEEDIT_X "gui/lineedit_x_cn"
#define SETTINGS_GUI_CN_LINEEDIT_Y "gui/lineedit_y_cn"
#define SETTINGS_GUI_CN_COMBOMODEL "gui/combomodel_cn"
#define SETTINGS_GUI_TABWIDGET "gui/tabwidget"
#define SETTINGS_GUI_SPLITTERLISTS_STATE "gui/splitterLists/state"
#define SETTINGS_GUI_SPLITTERGLOBAL_STATE "gui/splitterGlobal/state"
#define SETTINGS_GUI_TABLECOLUMNSIZE "gui/tablecolumnsize"
#define SETTINGS_GUI_SHORTCUT "shortcut/"
#define SETTINGS_GUI_ACTIONCHECKABLE "gui/actioncheckable/"
#define SETTINGS_DEFAULT_MEASURES_ITERATION "default_measures_iteration"
#define SETTINGS_DEFAULT_SETTLING_TIME "default_settling_time"

#endif // COMMON_H
