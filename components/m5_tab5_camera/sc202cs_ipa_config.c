/*
* SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
*
* SPDX-License-Identifier: ESPRESSIF MIT
*/

#include <string.h>
#include "esp_ipa.h"

typedef struct esp_video_ipa_index {
    const char *name;
    const esp_ipa_config_t *ipa_config;
} esp_video_ipa_index_t;

static const esp_ipa_ian_ct_basic_param_t s_ipa_ian_ct_SC202CS_basic_param[] = {
    {
        .a0 = 0.879032,
        .a1 = 0.290323
    },
    {
        .a0 = 0.790698,
        .a1 = 0.364341
    },
    {
        .a0 = 0.723077,
        .a1 = 0.415385
    },
    {
        .a0 = 0.656250,
        .a1 = 0.460938
    },
    {
        .a0 = 0.620968,
        .a1 = 0.500000
    },
    {
        .a0 = 0.584615,
        .a1 = 0.523077
    },
    {
        .a0 = 0.551181,
        .a1 = 0.543307
    },
    {
        .a0 = 0.526718,
        .a1 = 0.564885
    },
    {
        .a0 = 0.500000,
        .a1 = 0.580645
    },
    {
        .a0 = 0.476562,
        .a1 = 0.593750
    },
    {
        .a0 = 0.455285,
        .a1 = 0.601626
    },
    {
        .a0 = 0.443548,
        .a1 = 0.612903
    },
    {
        .a0 = 0.428571,
        .a1 = 0.626984
    },
    {
        .a0 = 0.418605,
        .a1 = 0.635659
    },
    {
        .a0 = 0.407692,
        .a1 = 0.638462
    },
    {
        .a0 = 0.380952,
        .a1 = 0.658730
    },
};

static const float s_esp_ipa_ian_ct_SC202CS_g_a2[] = {
    -237.746226, 2177.004255, -5984.610096, 8062.352386, 
};

static const esp_ipa_ian_ct_config_t s_esp_ipa_ian_ct_SC202CS_config = {
    .model = 2,
    .m_a0 = -0.229169,
    .m_a1 = -0.446364,
    .m_a2 = 0.859552,
    .f_n0 = 0.003300,
    .bp = s_ipa_ian_ct_SC202CS_basic_param,
    .bp_nums = ARRAY_SIZE(s_ipa_ian_ct_SC202CS_basic_param),
    .min_step = 1,
    .g_a0 = -0.332000,
    .g_a1 = -0.185800,
    .g_a2 = s_esp_ipa_ian_ct_SC202CS_g_a2,
    .g_a2_nums = ARRAY_SIZE(s_esp_ipa_ian_ct_SC202CS_g_a2)       
};

static const esp_ipa_ian_luma_ae_config_t s_esp_ipa_ian_luma_ae_SC202CS_config = {                 
    .weight = {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    },
};

static const float s_esp_ipa_ian_luma_env_speed_param_SC202CS_config[] = {
    -0.005463, -0.010018, 0.000000, 0.033241, 0.085583, 0.136704, 0.160734, 0.148777, 0.148777, 0.160734, 0.136704, 0.085583, 0.033241, 0.000000, -0.010018, -0.005463, 
};

static const esp_ipa_ian_luma_env_config_t s_esp_ipa_ian_luma_env_SC202CS_config = {
    .k = 250000.000000,
    .speed_param = s_esp_ipa_ian_luma_env_speed_param_SC202CS_config,
    .speed_param_size = ARRAY_SIZE(s_esp_ipa_ian_luma_env_speed_param_SC202CS_config),
    .weight = {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    },
};

static const esp_ipa_ian_luma_config_t s_esp_ipa_ian_luma_SC202CS_config = {
    .ae = &s_esp_ipa_ian_luma_ae_SC202CS_config,
    .env = &s_esp_ipa_ian_luma_env_SC202CS_config,
};

static const esp_ipa_ian_config_t s_ipa_ian_SC202CS_config = {
    .ct = &s_esp_ipa_ian_ct_SC202CS_config,
    .luma = &s_esp_ipa_ian_luma_SC202CS_config,
};

static const esp_ipa_awb_zone_t s_ipa_awb_zones_SC202CS[] = {
    { .type = ESP_IPA_AWB_ZONE_UHCT,  .rg_min = 0.32, .rg_max = 0.52, .bg_min = 0.63, .bg_max = 0.78, .enabled = true },
    { .type = ESP_IPA_AWB_ZONE_HCT,   .rg_min = 0.48, .rg_max = 0.58, .bg_min = 0.59, .bg_max = 0.63, .enabled = true },
    { .type = ESP_IPA_AWB_ZONE_MCT,   .rg_min = 0.55, .rg_max = 0.68, .bg_min = 0.52, .bg_max = 0.59, .enabled = true },
    { .type = ESP_IPA_AWB_ZONE_LCT,   .rg_min = 0.65, .rg_max = 0.82, .bg_min = 0.40, .bg_max = 0.52, .enabled = true },
    { .type = ESP_IPA_AWB_ZONE_ULCT,  .rg_min = 0.82, .rg_max = 1.05, .bg_min = 0.22, .bg_max = 0.40, .enabled = false },
};

static const esp_ipa_awb_ct_point_t s_ipa_awb_ref_points_SC202CS[] = {
    { .ct = 2567,  .rg = 0.9778, .bg = 0.2834, .radius = 0.02 },
    { .ct = 3673,  .rg = 0.7598, .bg = 0.4496, .radius = 0.02 },
    { .ct = 3946,  .rg = 0.7230, .bg = 0.4729, .radius = 0.02 },
    { .ct = 4377,  .rg = 0.6753, .bg = 0.5081, .radius = 0.03 },
    { .ct = 4862,  .rg = 0.6308, .bg = 0.5416, .radius = 0.04 },
    { .ct = 5625,  .rg = 0.5736, .bg = 0.5814, .radius = 0.04 },
    { .ct = 6113,  .rg = 0.5433, .bg = 0.6056, .radius = 0.03 },
    { .ct = 6575,  .rg = 0.5088, .bg = 0.6260, .radius = 0.03 },
    { .ct = 6970,  .rg = 0.4956, .bg = 0.6379, .radius = 0.02 },
    { .ct = 10000, .rg = 0.3763, .bg = 0.7215, .radius = 0.04 },
};

static const esp_ipa_awb_config_t s_ipa_awb_SC202CS_config = {
    .model = ESP_IPA_AWB_MODEL_2,
    .min_counted = 10,
    .min_red_gain_step = 0.05,
    .min_blue_gain_step = 0.05,
    .red_gain_scale = 1.1,
    .blue_gain_scale = 1.1,
    .range = {
        .green_max = 208,
        .green_min = 18,
        .rg_max = 0.97,
        .rg_min = 0.32,
        .bg_max = 0.80,
        .bg_min = 0.22
    },
    .green_luma_env = "dummy_awb_luma",
    .green_luma_init = 180,
    .green_luma_step_ratio = 0.3000,
    .new_w = 0.3,
    .prev_w = 0.7,
    .export_ct = true,
    .outlier_rg = 0.08,
    .outlier_bg = 0.08,
    .type_counter_max = 2000,
    .zones = s_ipa_awb_zones_SC202CS,
    .zones_count = ARRAY_SIZE(s_ipa_awb_zones_SC202CS),
    .ref_points = s_ipa_awb_ref_points_SC202CS,
    .ref_points_count = ARRAY_SIZE(s_ipa_awb_ref_points_SC202CS),
};

static const esp_ipa_agc_meter_light_threshold_t s_ipa_agc_meter_light_thresholds_SC202CS[] = {
    {
        .luma_threshold = 20,
        .weight_offset = 1,
    },
    {
        .luma_threshold = 55,
        .weight_offset = 2,
    },
    {
        .luma_threshold = 95,
        .weight_offset = 3,
    },
    {
        .luma_threshold = 155,
        .weight_offset = 4,
    },
    {
        .luma_threshold = 235,
        .weight_offset = 5,
    },
};

static const esp_ipa_agc_config_t s_ipa_agc_SC202CS_config = {
    .exposure_frame_delay = 3,
    .exposure_adjust_delay = 0,
    .gain_frame_delay = 3,
    .min_gain_step = 0.0300,
    .inc_gain_ratio = 0.3200,
    .dec_gain_ratio = 0.4200,
    .anti_flicker_mode = ESP_IPA_AGC_ANTI_FLICKER_PART,
    .ac_freq = 50,
    .luma_low = 56,
    .luma_high = 64,
    .luma_target = 62,
    .luma_low_threshold = 14,
    .luma_low_regions = 5,
    .luma_high_threshold = 239,
    .luma_high_regions = 3,
    .luma_weight_table = {
        1, 1, 2, 1, 1, 1, 2, 3, 2, 1, 1, 3, 4, 3, 1, 1, 2, 3, 2, 1, 1, 1, 2, 1, 1, 
    },
    .meter_mode = ESP_IPA_AGC_METER_HIGHLIGHT_PRIOR,
    .high_light_prior_config = {
        .luma_high_threshold = 202,
        .luma_low_threshold = 119,
        .weight_offset = 5,
        .luma_offset = -3
    },
    .low_light_prior_config = {
        .luma_high_threshold = 56,
        .luma_low_threshold = 48,
        .weight_offset = 5,
        .luma_offset = 1
    },
    .light_threshold_config = {
        .table = s_ipa_agc_meter_light_thresholds_SC202CS,
        .table_size = ARRAY_SIZE(s_ipa_agc_meter_light_thresholds_SC202CS)
    },
};

static const esp_ipa_adn_bf_t s_ipa_adn_bf_SC202CS_config[] = {
    {
        .gain = 1000,
        .bf = {
            .level = 3,
            .matrix = {
                {2, 4, 2},
                {4, 5, 4},
                {2, 4, 2}
            }
        }
    },
    {
        .gain = 4000,
        .bf = {
            .level = 4,
            .matrix = {
                {1, 3, 1},
                {3, 4, 3},
                {1, 3, 1}
            }
        }
    },
    {
        .gain = 8000,
        .bf = {
            .level = 5,
            .matrix = {
                {1, 3, 1},
                {3, 4, 3},
                {1, 3, 1}
            }
        }
    },
    {
        .gain = 16000,
        .bf = {
            .level = 8,
            .matrix = {
                {2, 3, 2},
                {3, 5, 3},
                {2, 3, 2}
            }
        }
    },
    {
        .gain = 24000,
        .bf = {
            .level = 9,
            .matrix = {
                {1, 2, 1},
                {2, 3, 2},
                {1, 2, 1}
            }
        }
    },
    {
        .gain = 32000,
        .bf = {
            .level = 10,
            .matrix = {
                {1, 2, 1},
                {2, 2, 2},
                {1, 2, 1}
            }
        }
    },
    {
        .gain = 64000,
        .bf = {
            .level = 12,
            .matrix = {
                {1, 2, 1},
                {2, 3, 2},
                {1, 2, 1}
            }
        }
    },
};

static const esp_ipa_adn_dm_t s_ipa_adn_dm_SC202CS_config[] = {
    {
        .gain = 1000,
        .dm = {
            .gradient_ratio = 1.5000
        }
    },
    {
        .gain = 4000,
        .dm = {
            .gradient_ratio = 1.2500
        }
    },
    {
        .gain = 8000,
        .dm = {
            .gradient_ratio = 1.0500
        }
    },
    {
        .gain = 12000,
        .dm = {
            .gradient_ratio = 1.0000
        }
    },
};

static const esp_ipa_adn_config_t s_ipa_adn_SC202CS_config = {
    .bf_table = s_ipa_adn_bf_SC202CS_config,
    .bf_table_size = ARRAY_SIZE(s_ipa_adn_bf_SC202CS_config),
    .dm_table = s_ipa_adn_dm_SC202CS_config,
    .dm_table_size = ARRAY_SIZE(s_ipa_adn_dm_SC202CS_config),
};

static const uint8_t s_aen_gamma_x[ISP_GAMMA_CURVE_POINTS_NUM] = {
    0, 17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255,
};
static const uint8_t s_aen_gamma_y[ISP_GAMMA_CURVE_POINTS_NUM] = {
    0, 1, 5, 11, 20, 31, 43, 59, 76, 95, 117, 140, 166, 193, 223, 255,
};

static const esp_ipa_aen_gamma_unit_t s_aen_gamma_table_SC202CS[] = {
    {
        .luma = 71.1,
        .gamma = {
            .red   = { .x = {0}, .y = {0} },
            .green = { .x = {0}, .y = {0} },
            .blue  = { .x = {0}, .y = {0} },
        },
    },
};

static const esp_ipa_aen_sharpen_t s_aen_sharpen_SC202CS[] = {
    {
        .gain = 1,
        .sharpen = {
            .h_thresh = 25,
            .l_thresh = 5,
            .h_coeff = 1.925,
            .m_coeff = 1.825,
            .matrix = {
                { 1, 2, 1 },
                { 2, 2, 2 },
                { 1, 2, 1 },
            },
        },
    },
    {
        .gain = 8000,
        .sharpen = {
            .h_thresh = 20,
            .l_thresh = 5,
            .h_coeff = 1.825,
            .m_coeff = 1.425,
            .matrix = {
                { 2, 2, 2 },
                { 2, 1, 2 },
                { 2, 2, 2 },
            },
        },
    },
    {
        .gain = 12000,
        .sharpen = {
            .h_thresh = 16,
            .l_thresh = 6,
            .h_coeff = 1.625,
            .m_coeff = 1.325,
            .matrix = {
                { 1, 1, 1 },
                { 1, 1, 1 },
                { 1, 1, 1 },
            },
        },
    },
    {
        .gain = 65000,
        .sharpen = {
            .h_thresh = 20,
            .l_thresh = 5,
            .h_coeff = 1.625,
            .m_coeff = 1.225,
            .matrix = {
                { 1, 1, 1 },
                { 1, 1, 1 },
                { 1, 1, 1 },
            },
        },
    },
};

static const esp_ipa_aen_con_t s_aen_con_SC202CS[] = {
    { .gain = 1,     .contrast = 132 },
    { .gain = 16000, .contrast = 130 },
    { .gain = 24000, .contrast = 128 },
    { .gain = 65000, .contrast = 126 },
};

static const esp_ipa_aen_gamma_config_t s_aen_gamma_config_SC202CS = {
    .model = ESP_IPA_AEN_GAMMA_MODEL_0,
    .luma_env = "ae.luma.avg",
    .luma_min_step = 16.0,
    .gamma_table = s_aen_gamma_table_SC202CS,
    .gamma_table_size = ARRAY_SIZE(s_aen_gamma_table_SC202CS),
};

static const esp_ipa_aen_config_t s_ipa_aen_SC202CS_config = {
    .gamma = &s_aen_gamma_config_SC202CS,
    .sharpen_table = s_aen_sharpen_SC202CS,
    .sharpen_table_size = ARRAY_SIZE(s_aen_sharpen_SC202CS),
    .con_table = s_aen_con_SC202CS,
    .con_table_size = ARRAY_SIZE(s_aen_con_SC202CS),
};

static const esp_ipa_acc_sat_t s_ipa_acc_sat_SC202CS_config[] = {
    {
        .color_temp = 0,
        .saturation = 128
    },
    {
        .color_temp = 4500,
        .saturation = 130
    },
};

static const esp_ipa_acc_ccm_unit_t s_esp_ipa_acc_ccm_SC202CS_table[] = {
    {
        .color_temp = 1200,
        .ccm = {
            .matrix = {
                { 1.0000, 0.0000, 0.0000 },
                { 0.0000, 1.0000, 0.0000 },
                { 0.0000, 0.0000, 1.0000 }
            }
        }
    },
    {
        .color_temp = 2567,
        .ccm = {
            .matrix = {
                { 1.2879,  0.0015, -0.2894 },
                { -0.2566, 1.4435, -0.1870 },
                { -0.2094, -0.3752, 1.5846 }
            }
        }
    },
    {
        .color_temp = 3673,
        .ccm = {
            .matrix = {
                { 1.2954, -0.1857, -0.1097 },
                { -0.2410, 1.3429, -0.1020 },
                { -0.0546, -0.3936, 1.4482 }
            }
        }
    },
    {
        .color_temp = 3946,
        .ccm = {
            .matrix = {
                { 1.3806, -0.2575, -0.1231 },
                { -0.2278, 1.3382, -0.1104 },
                { -0.0331, -0.4365, 1.4696 }
            }
        }
    },
    {
        .color_temp = 4377,
        .ccm = {
            .matrix = {
                { 1.3474, -0.2392, -0.1082 },
                { -0.2328, 1.3450, -0.1122 },
                { -0.0431, -0.3919, 1.4350 }
            }
        }
    },
    {
        .color_temp = 4862,
        .ccm = {
            .matrix = {
                { 1.4025, -0.2962, -0.1064 },
                { -0.2309, 1.3491, -0.1182 },
                { -0.0342, -0.4115, 1.4457 }
            }
        }
    },
    {
        .color_temp = 5625,
        .ccm = {
            .matrix = {
                { 1.4045, -0.3124, -0.0920 },
                { -0.2050, 1.3240, -0.1189 },
                { -0.0260, -0.3755, 1.4015 }
            }
        }
    },
    {
        .color_temp = 6113,
        .ccm = {
            .matrix = {
                { 1.3676, -0.2719, -0.0957 },
                { -0.1922, 1.3036, -0.1115 },
                { -0.0297, -0.3233, 1.3531 }
            }
        }
    },
    {
        .color_temp = 6575,
        .ccm = {
            .matrix = {
                { 1.3388, -0.2463, -0.0925 },
                { -0.1822, 1.2937, -0.1114 },
                { -0.0349, -0.2837, 1.3187 }
            }
        }
    },
    {
        .color_temp = 6970,
        .ccm = {
            .matrix = {
                { 1.3932, -0.3156, -0.0776 },
                { -0.1670, 1.2778, -0.1108 },
                { -0.0223, -0.3018, 1.3240 }
            }
        }
    },
    {
        .color_temp = 10000,
        .ccm = {
            .matrix = {
                { 1.3194, -0.2781, -0.0413 },
                { -0.1056, 1.1873, -0.0818 },
                { -0.0150, -0.1994, 1.2144 }
            }
        }
    },
    {
        .color_temp = 12000,
        .ccm = {
            .matrix = {
                { 1.0000, 0.0000, 0.0000 },
                { 0.0000, 1.0000, 0.0000 },
                { 0.0000, 0.0000, 1.0000 }
            }
        }
    },
};

static const esp_ipa_acc_ccm_config_t s_esp_ipa_acc_ccm_SC202CS_config = {
    .model = 0,
    .luma_env = "ae.luma.avg",
    .luma_low_threshold = 28.0000,
    .luma_low_ccm = {
        .matrix = {
            { 1.0000, 0.0000, 0.0000 },
            { 0.0000, 1.0000, 0.0000 },
            { 0.0000, 0.0000, 1.0000 }
        }
    }
    ,
    .ccm_table = s_esp_ipa_acc_ccm_SC202CS_table,
    .ccm_table_size = 12,
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_2410_config[] = {
    {.val = 821}, {.val = 714}, {.val = 624}, {.val = 549}, {.val = 487}, {.val = 439}, {.val = 405}, {.val = 382}, {.val = 373}, {.val = 365}, {.val = 359}, {.val = 368}, {.val = 385}, {.val = 414}, {.val = 451}, {.val = 500}, {.val = 563}, {.val = 641}, {.val = 732}, {.val = 836}, {.val = 836}, {.val = 758}, {.val = 655}, {.val = 566}, {.val = 495}, {.val = 438}, {.val = 394}, {.val = 361}, {.val = 340}, {.val = 332}, {.val = 326}, {.val = 319}, {.val = 327}, {.val = 342}, {.val = 366}, {.val = 402}, {.val = 449}, {.val = 508}, {.val = 583}, {.val = 672}, {.val = 775}, {.val = 775}, {.val = 707}, {.val = 607}, {.val = 523}, {.val = 455}, {.val = 401}, {.val = 360}, {.val = 329}, {.val = 308}, {.val = 298}, {.val = 291}, {.val = 291}, {.val = 298}, {.val = 311}, {.val = 334}, {.val = 366}, {.val = 411}, {.val = 468}, {.val = 540}, {.val = 626}, {.val = 727}, {.val = 727}, {.val = 672}, {.val = 573}, {.val = 492}, {.val = 427}, {.val = 376}, {.val = 336}, {.val = 308}, {.val = 288}, {.val = 277}, {.val = 271}, {.val = 271}, {.val = 277}, {.val = 291}, {.val = 312}, {.val = 343}, {.val = 385}, {.val = 440}, {.val = 510}, {.val = 594}, {.val = 692}, {.val = 692}, {.val = 651}, {.val = 554}, {.val = 474}, {.val = 410}, {.val = 360}, {.val = 322}, {.val = 295}, {.val = 276}, {.val = 265}, {.val = 259}, {.val = 259}, {.val = 265}, {.val = 278}, {.val = 299}, {.val = 329}, {.val = 370}, {.val = 423}, {.val = 492}, {.val = 574}, {.val = 672}, {.val = 672}, {.val = 642}, {.val = 546}, {.val = 467}, {.val = 404}, {.val = 354}, {.val = 316}, {.val = 290}, {.val = 271}, {.val = 260}, {.val = 254}, {.val = 255}, {.val = 260}, {.val = 273}, {.val = 294}, {.val = 323}, {.val = 364}, {.val = 417}, {.val = 484}, {.val = 566}, {.val = 663}, {.val = 663}, {.val = 643}, {.val = 549}, {.val = 469}, {.val = 407}, {.val = 357}, {.val = 320}, {.val = 292}, {.val = 273}, {.val = 261}, {.val = 256}, {.val = 256}, {.val = 262}, {.val = 275}, {.val = 296}, {.val = 326}, {.val = 367}, {.val = 420}, {.val = 488}, {.val = 569}, {.val = 667}, {.val = 667}, {.val = 659}, {.val = 564}, {.val = 483}, {.val = 419}, {.val = 368}, {.val = 329}, {.val = 301}, {.val = 282}, {.val = 269}, {.val = 264}, {.val = 264}, {.val = 271}, {.val = 284}, {.val = 305}, {.val = 337}, {.val = 378}, {.val = 433}, {.val = 502}, {.val = 585}, {.val = 681}, {.val = 681}, {.val = 688}, {.val = 590}, {.val = 508}, {.val = 442}, {.val = 389}, {.val = 349}, {.val = 319}, {.val = 299}, {.val = 286}, {.val = 280}, {.val = 280}, {.val = 287}, {.val = 302}, {.val = 325}, {.val = 357}, {.val = 401}, {.val = 457}, {.val = 527}, {.val = 612}, {.val = 712}, {.val = 712}, {.val = 731}, {.val = 631}, {.val = 546}, {.val = 476}, {.val = 421}, {.val = 378}, {.val = 347}, {.val = 325}, {.val = 312}, {.val = 305}, {.val = 305}, {.val = 313}, {.val = 329}, {.val = 353}, {.val = 388}, {.val = 435}, {.val = 494}, {.val = 567}, {.val = 653}, {.val = 755}, {.val = 755}, {.val = 785}, {.val = 685}, {.val = 597}, {.val = 524}, {.val = 466}, {.val = 421}, {.val = 386}, {.val = 362}, {.val = 348}, {.val = 340}, {.val = 341}, {.val = 350}, {.val = 368}, {.val = 394}, {.val = 431}, {.val = 480}, {.val = 543}, {.val = 619}, {.val = 708}, {.val = 807}, {.val = 807}, {.val = 830}, {.val = 726}, {.val = 636}, {.val = 561}, {.val = 499}, {.val = 453}, {.val = 417}, {.val = 392}, {.val = 377}, {.val = 369}, {.val = 371}, {.val = 379}, {.val = 398}, {.val = 427}, {.val = 464}, {.val = 515}, {.val = 579}, {.val = 658}, {.val = 750}, {.val = 851}, {.val = 851}, {.val = 830}, {.val = 726}, {.val = 636}, {.val = 561}, {.val = 499}, {.val = 453}, {.val = 417}, {.val = 392}, {.val = 377}, {.val = 369}, {.val = 371}, {.val = 379}, {.val = 398}, {.val = 427}, {.val = 464}, {.val = 515}, {.val = 579}, {.val = 658}, {.val = 750}, {.val = 851}, {.val = 851}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_2410_config[] = {
    {.val = 590}, {.val = 535}, {.val = 486}, {.val = 445}, {.val = 410}, {.val = 383}, {.val = 362}, {.val = 348}, {.val = 344}, {.val = 338}, {.val = 330}, {.val = 335}, {.val = 345}, {.val = 362}, {.val = 383}, {.val = 408}, {.val = 441}, {.val = 481}, {.val = 526}, {.val = 577}, {.val = 577}, {.val = 556}, {.val = 500}, {.val = 453}, {.val = 413}, {.val = 380}, {.val = 353}, {.val = 333}, {.val = 320}, {.val = 315}, {.val = 310}, {.val = 304}, {.val = 308}, {.val = 317}, {.val = 332}, {.val = 352}, {.val = 378}, {.val = 411}, {.val = 449}, {.val = 494}, {.val = 546}, {.val = 546}, {.val = 528}, {.val = 474}, {.val = 427}, {.val = 388}, {.val = 356}, {.val = 329}, {.val = 310}, {.val = 296}, {.val = 289}, {.val = 285}, {.val = 283}, {.val = 287}, {.val = 296}, {.val = 309}, {.val = 329}, {.val = 354}, {.val = 386}, {.val = 425}, {.val = 469}, {.val = 521}, {.val = 521}, {.val = 508}, {.val = 454}, {.val = 408}, {.val = 369}, {.val = 338}, {.val = 313}, {.val = 295}, {.val = 281}, {.val = 273}, {.val = 269}, {.val = 269}, {.val = 272}, {.val = 281}, {.val = 294}, {.val = 312}, {.val = 337}, {.val = 369}, {.val = 407}, {.val = 451}, {.val = 502}, {.val = 502}, {.val = 496}, {.val = 442}, {.val = 396}, {.val = 358}, {.val = 327}, {.val = 303}, {.val = 285}, {.val = 272}, {.val = 264}, {.val = 260}, {.val = 260}, {.val = 263}, {.val = 271}, {.val = 284}, {.val = 302}, {.val = 327}, {.val = 358}, {.val = 396}, {.val = 439}, {.val = 492}, {.val = 492}, {.val = 489}, {.val = 436}, {.val = 391}, {.val = 353}, {.val = 322}, {.val = 298}, {.val = 280}, {.val = 267}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 267}, {.val = 279}, {.val = 298}, {.val = 322}, {.val = 353}, {.val = 391}, {.val = 435}, {.val = 486}, {.val = 486}, {.val = 491}, {.val = 437}, {.val = 392}, {.val = 354}, {.val = 323}, {.val = 300}, {.val = 281}, {.val = 268}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 260}, {.val = 267}, {.val = 280}, {.val = 299}, {.val = 324}, {.val = 355}, {.val = 393}, {.val = 437}, {.val = 488}, {.val = 488}, {.val = 497}, {.val = 445}, {.val = 399}, {.val = 361}, {.val = 330}, {.val = 306}, {.val = 287}, {.val = 274}, {.val = 265}, {.val = 261}, {.val = 262}, {.val = 265}, {.val = 273}, {.val = 287}, {.val = 307}, {.val = 332}, {.val = 363}, {.val = 402}, {.val = 446}, {.val = 497}, {.val = 497}, {.val = 512}, {.val = 458}, {.val = 413}, {.val = 375}, {.val = 343}, {.val = 318}, {.val = 299}, {.val = 285}, {.val = 276}, {.val = 272}, {.val = 272}, {.val = 276}, {.val = 285}, {.val = 300}, {.val = 319}, {.val = 345}, {.val = 377}, {.val = 415}, {.val = 460}, {.val = 512}, {.val = 512}, {.val = 535}, {.val = 480}, {.val = 434}, {.val = 394}, {.val = 362}, {.val = 336}, {.val = 317}, {.val = 302}, {.val = 293}, {.val = 289}, {.val = 289}, {.val = 294}, {.val = 304}, {.val = 318}, {.val = 339}, {.val = 365}, {.val = 397}, {.val = 437}, {.val = 482}, {.val = 533}, {.val = 533}, {.val = 563}, {.val = 508}, {.val = 461}, {.val = 422}, {.val = 388}, {.val = 362}, {.val = 341}, {.val = 326}, {.val = 317}, {.val = 312}, {.val = 313}, {.val = 318}, {.val = 328}, {.val = 344}, {.val = 365}, {.val = 391}, {.val = 425}, {.val = 464}, {.val = 509}, {.val = 561}, {.val = 561}, {.val = 584}, {.val = 531}, {.val = 482}, {.val = 442}, {.val = 408}, {.val = 381}, {.val = 361}, {.val = 345}, {.val = 336}, {.val = 330}, {.val = 331}, {.val = 336}, {.val = 347}, {.val = 362}, {.val = 385}, {.val = 412}, {.val = 445}, {.val = 483}, {.val = 529}, {.val = 582}, {.val = 582}, {.val = 584}, {.val = 531}, {.val = 482}, {.val = 442}, {.val = 408}, {.val = 381}, {.val = 361}, {.val = 345}, {.val = 336}, {.val = 330}, {.val = 331}, {.val = 336}, {.val = 347}, {.val = 362}, {.val = 385}, {.val = 412}, {.val = 445}, {.val = 483}, {.val = 529}, {.val = 582}, {.val = 582}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_2410_config[] = {
    {.val = 582}, {.val = 527}, {.val = 478}, {.val = 437}, {.val = 403}, {.val = 375}, {.val = 354}, {.val = 340}, {.val = 336}, {.val = 330}, {.val = 324}, {.val = 329}, {.val = 339}, {.val = 356}, {.val = 378}, {.val = 404}, {.val = 438}, {.val = 478}, {.val = 524}, {.val = 575}, {.val = 575}, {.val = 552}, {.val = 497}, {.val = 449}, {.val = 409}, {.val = 376}, {.val = 349}, {.val = 328}, {.val = 315}, {.val = 309}, {.val = 305}, {.val = 299}, {.val = 303}, {.val = 313}, {.val = 328}, {.val = 349}, {.val = 375}, {.val = 409}, {.val = 448}, {.val = 494}, {.val = 545}, {.val = 545}, {.val = 529}, {.val = 474}, {.val = 426}, {.val = 387}, {.val = 354}, {.val = 328}, {.val = 308}, {.val = 293}, {.val = 286}, {.val = 281}, {.val = 280}, {.val = 284}, {.val = 293}, {.val = 307}, {.val = 327}, {.val = 353}, {.val = 386}, {.val = 425}, {.val = 471}, {.val = 523}, {.val = 523}, {.val = 513}, {.val = 457}, {.val = 411}, {.val = 371}, {.val = 339}, {.val = 313}, {.val = 294}, {.val = 280}, {.val = 272}, {.val = 268}, {.val = 267}, {.val = 271}, {.val = 279}, {.val = 293}, {.val = 312}, {.val = 338}, {.val = 370}, {.val = 408}, {.val = 454}, {.val = 506}, {.val = 506}, {.val = 503}, {.val = 448}, {.val = 401}, {.val = 362}, {.val = 330}, {.val = 305}, {.val = 285}, {.val = 272}, {.val = 263}, {.val = 259}, {.val = 259}, {.val = 263}, {.val = 271}, {.val = 284}, {.val = 303}, {.val = 329}, {.val = 360}, {.val = 399}, {.val = 444}, {.val = 496}, {.val = 496}, {.val = 499}, {.val = 445}, {.val = 397}, {.val = 358}, {.val = 326}, {.val = 301}, {.val = 282}, {.val = 268}, {.val = 260}, {.val = 255}, {.val = 255}, {.val = 259}, {.val = 267}, {.val = 280}, {.val = 300}, {.val = 325}, {.val = 357}, {.val = 395}, {.val = 440}, {.val = 492}, {.val = 492}, {.val = 502}, {.val = 447}, {.val = 401}, {.val = 360}, {.val = 328}, {.val = 303}, {.val = 284}, {.val = 270}, {.val = 261}, {.val = 256}, {.val = 256}, {.val = 260}, {.val = 268}, {.val = 282}, {.val = 301}, {.val = 327}, {.val = 359}, {.val = 397}, {.val = 443}, {.val = 494}, {.val = 494}, {.val = 512}, {.val = 457}, {.val = 408}, {.val = 369}, {.val = 336}, {.val = 310}, {.val = 290}, {.val = 276}, {.val = 267}, {.val = 262}, {.val = 261}, {.val = 266}, {.val = 275}, {.val = 289}, {.val = 308}, {.val = 334}, {.val = 367}, {.val = 406}, {.val = 452}, {.val = 504}, {.val = 504}, {.val = 530}, {.val = 473}, {.val = 424}, {.val = 383}, {.val = 350}, {.val = 323}, {.val = 302}, {.val = 288}, {.val = 278}, {.val = 273}, {.val = 273}, {.val = 277}, {.val = 287}, {.val = 301}, {.val = 322}, {.val = 349}, {.val = 382}, {.val = 420}, {.val = 467}, {.val = 518}, {.val = 518}, {.val = 553}, {.val = 497}, {.val = 447}, {.val = 405}, {.val = 371}, {.val = 343}, {.val = 322}, {.val = 306}, {.val = 296}, {.val = 290}, {.val = 290}, {.val = 295}, {.val = 305}, {.val = 320}, {.val = 342}, {.val = 369}, {.val = 402}, {.val = 443}, {.val = 489}, {.val = 540}, {.val = 540}, {.val = 585}, {.val = 527}, {.val = 477}, {.val = 434}, {.val = 398}, {.val = 369}, {.val = 347}, {.val = 330}, {.val = 320}, {.val = 314}, {.val = 314}, {.val = 319}, {.val = 330}, {.val = 346}, {.val = 368}, {.val = 396}, {.val = 430}, {.val = 470}, {.val = 516}, {.val = 569}, {.val = 569}, {.val = 608}, {.val = 551}, {.val = 500}, {.val = 455}, {.val = 419}, {.val = 389}, {.val = 368}, {.val = 350}, {.val = 340}, {.val = 334}, {.val = 333}, {.val = 339}, {.val = 349}, {.val = 365}, {.val = 388}, {.val = 417}, {.val = 449}, {.val = 492}, {.val = 536}, {.val = 589}, {.val = 589}, {.val = 608}, {.val = 551}, {.val = 500}, {.val = 455}, {.val = 419}, {.val = 389}, {.val = 368}, {.val = 350}, {.val = 340}, {.val = 334}, {.val = 333}, {.val = 339}, {.val = 349}, {.val = 365}, {.val = 388}, {.val = 417}, {.val = 449}, {.val = 492}, {.val = 536}, {.val = 589}, {.val = 589}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_2410_config[] = {
    {.val = 535}, {.val = 490}, {.val = 449}, {.val = 417}, {.val = 387}, {.val = 366}, {.val = 347}, {.val = 335}, {.val = 334}, {.val = 328}, {.val = 321}, {.val = 325}, {.val = 334}, {.val = 349}, {.val = 366}, {.val = 388}, {.val = 415}, {.val = 448}, {.val = 486}, {.val = 528}, {.val = 528}, {.val = 510}, {.val = 464}, {.val = 422}, {.val = 390}, {.val = 363}, {.val = 340}, {.val = 322}, {.val = 312}, {.val = 308}, {.val = 304}, {.val = 298}, {.val = 301}, {.val = 310}, {.val = 323}, {.val = 340}, {.val = 362}, {.val = 391}, {.val = 422}, {.val = 460}, {.val = 500}, {.val = 500}, {.val = 489}, {.val = 443}, {.val = 404}, {.val = 371}, {.val = 343}, {.val = 320}, {.val = 303}, {.val = 290}, {.val = 285}, {.val = 281}, {.val = 280}, {.val = 284}, {.val = 291}, {.val = 304}, {.val = 321}, {.val = 342}, {.val = 370}, {.val = 403}, {.val = 441}, {.val = 482}, {.val = 482}, {.val = 472}, {.val = 429}, {.val = 389}, {.val = 356}, {.val = 328}, {.val = 306}, {.val = 289}, {.val = 278}, {.val = 271}, {.val = 268}, {.val = 268}, {.val = 271}, {.val = 278}, {.val = 290}, {.val = 307}, {.val = 328}, {.val = 356}, {.val = 390}, {.val = 426}, {.val = 469}, {.val = 469}, {.val = 464}, {.val = 419}, {.val = 381}, {.val = 348}, {.val = 320}, {.val = 299}, {.val = 281}, {.val = 270}, {.val = 263}, {.val = 260}, {.val = 260}, {.val = 262}, {.val = 270}, {.val = 282}, {.val = 298}, {.val = 321}, {.val = 348}, {.val = 380}, {.val = 418}, {.val = 459}, {.val = 459}, {.val = 461}, {.val = 415}, {.val = 376}, {.val = 344}, {.val = 318}, {.val = 295}, {.val = 279}, {.val = 267}, {.val = 259}, {.val = 256}, {.val = 256}, {.val = 260}, {.val = 266}, {.val = 278}, {.val = 295}, {.val = 317}, {.val = 344}, {.val = 378}, {.val = 414}, {.val = 457}, {.val = 457}, {.val = 462}, {.val = 418}, {.val = 379}, {.val = 345}, {.val = 318}, {.val = 296}, {.val = 280}, {.val = 268}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 260}, {.val = 267}, {.val = 279}, {.val = 297}, {.val = 319}, {.val = 347}, {.val = 379}, {.val = 417}, {.val = 458}, {.val = 458}, {.val = 469}, {.val = 423}, {.val = 386}, {.val = 352}, {.val = 325}, {.val = 302}, {.val = 285}, {.val = 273}, {.val = 265}, {.val = 262}, {.val = 263}, {.val = 266}, {.val = 273}, {.val = 286}, {.val = 304}, {.val = 326}, {.val = 355}, {.val = 387}, {.val = 424}, {.val = 466}, {.val = 466}, {.val = 487}, {.val = 437}, {.val = 397}, {.val = 364}, {.val = 336}, {.val = 313}, {.val = 296}, {.val = 284}, {.val = 276}, {.val = 272}, {.val = 273}, {.val = 276}, {.val = 285}, {.val = 298}, {.val = 317}, {.val = 339}, {.val = 368}, {.val = 400}, {.val = 438}, {.val = 478}, {.val = 478}, {.val = 510}, {.val = 458}, {.val = 416}, {.val = 381}, {.val = 353}, {.val = 330}, {.val = 313}, {.val = 300}, {.val = 292}, {.val = 288}, {.val = 288}, {.val = 293}, {.val = 302}, {.val = 315}, {.val = 334}, {.val = 357}, {.val = 385}, {.val = 418}, {.val = 454}, {.val = 498}, {.val = 498}, {.val = 537}, {.val = 488}, {.val = 443}, {.val = 404}, {.val = 376}, {.val = 353}, {.val = 336}, {.val = 322}, {.val = 314}, {.val = 310}, {.val = 310}, {.val = 315}, {.val = 325}, {.val = 340}, {.val = 358}, {.val = 381}, {.val = 410}, {.val = 443}, {.val = 481}, {.val = 523}, {.val = 523}, {.val = 557}, {.val = 511}, {.val = 464}, {.val = 424}, {.val = 392}, {.val = 370}, {.val = 353}, {.val = 338}, {.val = 332}, {.val = 328}, {.val = 328}, {.val = 333}, {.val = 343}, {.val = 358}, {.val = 375}, {.val = 399}, {.val = 429}, {.val = 461}, {.val = 500}, {.val = 539}, {.val = 539}, {.val = 557}, {.val = 511}, {.val = 464}, {.val = 424}, {.val = 392}, {.val = 370}, {.val = 353}, {.val = 338}, {.val = 332}, {.val = 328}, {.val = 328}, {.val = 333}, {.val = 343}, {.val = 358}, {.val = 375}, {.val = 399}, {.val = 429}, {.val = 461}, {.val = 500}, {.val = 539}, {.val = 539}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_4015_config[] = {
    {.val = 776}, {.val = 680}, {.val = 600}, {.val = 532}, {.val = 475}, {.val = 431}, {.val = 399}, {.val = 377}, {.val = 368}, {.val = 361}, {.val = 355}, {.val = 363}, {.val = 379}, {.val = 406}, {.val = 439}, {.val = 485}, {.val = 543}, {.val = 612}, {.val = 693}, {.val = 783}, {.val = 783}, {.val = 718}, {.val = 626}, {.val = 546}, {.val = 482}, {.val = 429}, {.val = 387}, {.val = 357}, {.val = 338}, {.val = 329}, {.val = 324}, {.val = 317}, {.val = 324}, {.val = 339}, {.val = 362}, {.val = 394}, {.val = 438}, {.val = 492}, {.val = 560}, {.val = 640}, {.val = 730}, {.val = 730}, {.val = 673}, {.val = 583}, {.val = 506}, {.val = 445}, {.val = 394}, {.val = 355}, {.val = 327}, {.val = 306}, {.val = 296}, {.val = 290}, {.val = 289}, {.val = 296}, {.val = 309}, {.val = 330}, {.val = 361}, {.val = 402}, {.val = 455}, {.val = 521}, {.val = 598}, {.val = 687}, {.val = 687}, {.val = 641}, {.val = 553}, {.val = 478}, {.val = 418}, {.val = 370}, {.val = 333}, {.val = 306}, {.val = 288}, {.val = 276}, {.val = 271}, {.val = 271}, {.val = 277}, {.val = 290}, {.val = 309}, {.val = 339}, {.val = 379}, {.val = 430}, {.val = 493}, {.val = 568}, {.val = 658}, {.val = 658}, {.val = 622}, {.val = 535}, {.val = 462}, {.val = 403}, {.val = 356}, {.val = 320}, {.val = 294}, {.val = 276}, {.val = 265}, {.val = 260}, {.val = 260}, {.val = 265}, {.val = 277}, {.val = 297}, {.val = 326}, {.val = 364}, {.val = 414}, {.val = 475}, {.val = 549}, {.val = 638}, {.val = 638}, {.val = 613}, {.val = 528}, {.val = 455}, {.val = 396}, {.val = 350}, {.val = 314}, {.val = 289}, {.val = 271}, {.val = 260}, {.val = 255}, {.val = 255}, {.val = 261}, {.val = 273}, {.val = 292}, {.val = 320}, {.val = 358}, {.val = 407}, {.val = 469}, {.val = 543}, {.val = 630}, {.val = 630}, {.val = 613}, {.val = 530}, {.val = 458}, {.val = 399}, {.val = 352}, {.val = 317}, {.val = 290}, {.val = 272}, {.val = 261}, {.val = 256}, {.val = 256}, {.val = 262}, {.val = 274}, {.val = 294}, {.val = 322}, {.val = 360}, {.val = 410}, {.val = 472}, {.val = 547}, {.val = 633}, {.val = 633}, {.val = 629}, {.val = 543}, {.val = 470}, {.val = 411}, {.val = 363}, {.val = 326}, {.val = 300}, {.val = 281}, {.val = 269}, {.val = 263}, {.val = 264}, {.val = 270}, {.val = 283}, {.val = 303}, {.val = 333}, {.val = 372}, {.val = 422}, {.val = 486}, {.val = 561}, {.val = 648}, {.val = 648}, {.val = 655}, {.val = 567}, {.val = 493}, {.val = 432}, {.val = 383}, {.val = 345}, {.val = 317}, {.val = 297}, {.val = 285}, {.val = 279}, {.val = 279}, {.val = 286}, {.val = 300}, {.val = 322}, {.val = 352}, {.val = 393}, {.val = 445}, {.val = 508}, {.val = 585}, {.val = 671}, {.val = 671}, {.val = 692}, {.val = 603}, {.val = 526}, {.val = 462}, {.val = 412}, {.val = 373}, {.val = 343}, {.val = 322}, {.val = 308}, {.val = 303}, {.val = 303}, {.val = 311}, {.val = 326}, {.val = 348}, {.val = 381}, {.val = 424}, {.val = 478}, {.val = 545}, {.val = 622}, {.val = 711}, {.val = 711}, {.val = 741}, {.val = 651}, {.val = 573}, {.val = 508}, {.val = 454}, {.val = 411}, {.val = 380}, {.val = 358}, {.val = 343}, {.val = 337}, {.val = 337}, {.val = 346}, {.val = 362}, {.val = 386}, {.val = 421}, {.val = 466}, {.val = 522}, {.val = 590}, {.val = 670}, {.val = 760}, {.val = 760}, {.val = 776}, {.val = 688}, {.val = 611}, {.val = 540}, {.val = 485}, {.val = 442}, {.val = 408}, {.val = 385}, {.val = 370}, {.val = 364}, {.val = 365}, {.val = 374}, {.val = 390}, {.val = 415}, {.val = 453}, {.val = 497}, {.val = 555}, {.val = 626}, {.val = 705}, {.val = 794}, {.val = 794}, {.val = 776}, {.val = 688}, {.val = 611}, {.val = 540}, {.val = 485}, {.val = 442}, {.val = 408}, {.val = 385}, {.val = 370}, {.val = 364}, {.val = 365}, {.val = 374}, {.val = 390}, {.val = 415}, {.val = 453}, {.val = 497}, {.val = 555}, {.val = 626}, {.val = 705}, {.val = 794}, {.val = 794}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_4015_config[] = {
    {.val = 574}, {.val = 522}, {.val = 476}, {.val = 437}, {.val = 405}, {.val = 379}, {.val = 358}, {.val = 344}, {.val = 340}, {.val = 335}, {.val = 328}, {.val = 332}, {.val = 341}, {.val = 357}, {.val = 378}, {.val = 401}, {.val = 432}, {.val = 469}, {.val = 512}, {.val = 559}, {.val = 559}, {.val = 542}, {.val = 490}, {.val = 445}, {.val = 406}, {.val = 375}, {.val = 350}, {.val = 330}, {.val = 318}, {.val = 313}, {.val = 308}, {.val = 302}, {.val = 306}, {.val = 315}, {.val = 328}, {.val = 348}, {.val = 372}, {.val = 403}, {.val = 440}, {.val = 483}, {.val = 529}, {.val = 529}, {.val = 516}, {.val = 464}, {.val = 420}, {.val = 383}, {.val = 352}, {.val = 327}, {.val = 309}, {.val = 295}, {.val = 288}, {.val = 284}, {.val = 282}, {.val = 286}, {.val = 294}, {.val = 307}, {.val = 325}, {.val = 350}, {.val = 380}, {.val = 416}, {.val = 458}, {.val = 507}, {.val = 507}, {.val = 498}, {.val = 445}, {.val = 402}, {.val = 366}, {.val = 336}, {.val = 312}, {.val = 294}, {.val = 281}, {.val = 273}, {.val = 269}, {.val = 269}, {.val = 272}, {.val = 280}, {.val = 292}, {.val = 310}, {.val = 334}, {.val = 364}, {.val = 400}, {.val = 442}, {.val = 490}, {.val = 490}, {.val = 486}, {.val = 435}, {.val = 391}, {.val = 355}, {.val = 325}, {.val = 302}, {.val = 284}, {.val = 272}, {.val = 264}, {.val = 261}, {.val = 260}, {.val = 263}, {.val = 271}, {.val = 283}, {.val = 301}, {.val = 324}, {.val = 354}, {.val = 390}, {.val = 431}, {.val = 479}, {.val = 479}, {.val = 479}, {.val = 429}, {.val = 386}, {.val = 350}, {.val = 321}, {.val = 297}, {.val = 280}, {.val = 268}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 266}, {.val = 279}, {.val = 296}, {.val = 320}, {.val = 349}, {.val = 385}, {.val = 426}, {.val = 474}, {.val = 474}, {.val = 480}, {.val = 430}, {.val = 387}, {.val = 351}, {.val = 321}, {.val = 298}, {.val = 281}, {.val = 268}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 260}, {.val = 267}, {.val = 279}, {.val = 298}, {.val = 321}, {.val = 350}, {.val = 386}, {.val = 428}, {.val = 476}, {.val = 476}, {.val = 487}, {.val = 437}, {.val = 394}, {.val = 358}, {.val = 328}, {.val = 305}, {.val = 286}, {.val = 273}, {.val = 265}, {.val = 261}, {.val = 261}, {.val = 265}, {.val = 273}, {.val = 286}, {.val = 305}, {.val = 328}, {.val = 358}, {.val = 394}, {.val = 436}, {.val = 484}, {.val = 484}, {.val = 499}, {.val = 450}, {.val = 407}, {.val = 370}, {.val = 340}, {.val = 316}, {.val = 297}, {.val = 284}, {.val = 276}, {.val = 271}, {.val = 272}, {.val = 275}, {.val = 284}, {.val = 298}, {.val = 317}, {.val = 341}, {.val = 371}, {.val = 407}, {.val = 449}, {.val = 497}, {.val = 497}, {.val = 520}, {.val = 469}, {.val = 426}, {.val = 388}, {.val = 358}, {.val = 333}, {.val = 315}, {.val = 300}, {.val = 291}, {.val = 287}, {.val = 287}, {.val = 292}, {.val = 301}, {.val = 316}, {.val = 335}, {.val = 359}, {.val = 390}, {.val = 427}, {.val = 469}, {.val = 518}, {.val = 518}, {.val = 545}, {.val = 495}, {.val = 451}, {.val = 413}, {.val = 382}, {.val = 357}, {.val = 337}, {.val = 323}, {.val = 314}, {.val = 310}, {.val = 309}, {.val = 314}, {.val = 324}, {.val = 339}, {.val = 359}, {.val = 384}, {.val = 415}, {.val = 452}, {.val = 494}, {.val = 542}, {.val = 542}, {.val = 565}, {.val = 515}, {.val = 470}, {.val = 431}, {.val = 400}, {.val = 374}, {.val = 355}, {.val = 340}, {.val = 331}, {.val = 327}, {.val = 327}, {.val = 332}, {.val = 342}, {.val = 357}, {.val = 377}, {.val = 403}, {.val = 434}, {.val = 471}, {.val = 512}, {.val = 562}, {.val = 562}, {.val = 565}, {.val = 515}, {.val = 470}, {.val = 431}, {.val = 400}, {.val = 374}, {.val = 355}, {.val = 340}, {.val = 331}, {.val = 327}, {.val = 327}, {.val = 332}, {.val = 342}, {.val = 357}, {.val = 377}, {.val = 403}, {.val = 434}, {.val = 471}, {.val = 512}, {.val = 562}, {.val = 562}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_4015_config[] = {
    {.val = 562}, {.val = 511}, {.val = 466}, {.val = 428}, {.val = 397}, {.val = 370}, {.val = 351}, {.val = 338}, {.val = 334}, {.val = 328}, {.val = 321}, {.val = 326}, {.val = 336}, {.val = 351}, {.val = 372}, {.val = 396}, {.val = 427}, {.val = 463}, {.val = 505}, {.val = 552}, {.val = 552}, {.val = 534}, {.val = 484}, {.val = 439}, {.val = 401}, {.val = 370}, {.val = 345}, {.val = 326}, {.val = 313}, {.val = 308}, {.val = 304}, {.val = 297}, {.val = 301}, {.val = 310}, {.val = 325}, {.val = 344}, {.val = 369}, {.val = 399}, {.val = 435}, {.val = 477}, {.val = 524}, {.val = 524}, {.val = 513}, {.val = 462}, {.val = 418}, {.val = 381}, {.val = 350}, {.val = 325}, {.val = 306}, {.val = 292}, {.val = 285}, {.val = 280}, {.val = 279}, {.val = 283}, {.val = 291}, {.val = 304}, {.val = 323}, {.val = 348}, {.val = 378}, {.val = 414}, {.val = 455}, {.val = 502}, {.val = 502}, {.val = 498}, {.val = 447}, {.val = 403}, {.val = 366}, {.val = 335}, {.val = 311}, {.val = 293}, {.val = 279}, {.val = 271}, {.val = 267}, {.val = 267}, {.val = 270}, {.val = 278}, {.val = 291}, {.val = 309}, {.val = 333}, {.val = 362}, {.val = 398}, {.val = 440}, {.val = 487}, {.val = 487}, {.val = 489}, {.val = 438}, {.val = 393}, {.val = 357}, {.val = 326}, {.val = 303}, {.val = 284}, {.val = 271}, {.val = 263}, {.val = 259}, {.val = 259}, {.val = 262}, {.val = 270}, {.val = 282}, {.val = 300}, {.val = 324}, {.val = 354}, {.val = 389}, {.val = 430}, {.val = 478}, {.val = 478}, {.val = 486}, {.val = 435}, {.val = 391}, {.val = 354}, {.val = 323}, {.val = 299}, {.val = 281}, {.val = 268}, {.val = 259}, {.val = 256}, {.val = 255}, {.val = 258}, {.val = 266}, {.val = 279}, {.val = 296}, {.val = 320}, {.val = 350}, {.val = 386}, {.val = 426}, {.val = 474}, {.val = 474}, {.val = 488}, {.val = 438}, {.val = 393}, {.val = 356}, {.val = 325}, {.val = 301}, {.val = 282}, {.val = 269}, {.val = 261}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 267}, {.val = 280}, {.val = 298}, {.val = 322}, {.val = 352}, {.val = 388}, {.val = 429}, {.val = 476}, {.val = 476}, {.val = 497}, {.val = 446}, {.val = 402}, {.val = 364}, {.val = 333}, {.val = 308}, {.val = 289}, {.val = 275}, {.val = 266}, {.val = 262}, {.val = 261}, {.val = 265}, {.val = 273}, {.val = 286}, {.val = 305}, {.val = 329}, {.val = 360}, {.val = 396}, {.val = 437}, {.val = 484}, {.val = 484}, {.val = 514}, {.val = 462}, {.val = 416}, {.val = 378}, {.val = 347}, {.val = 321}, {.val = 301}, {.val = 287}, {.val = 277}, {.val = 273}, {.val = 272}, {.val = 276}, {.val = 285}, {.val = 299}, {.val = 319}, {.val = 343}, {.val = 374}, {.val = 411}, {.val = 452}, {.val = 499}, {.val = 499}, {.val = 535}, {.val = 484}, {.val = 437}, {.val = 399}, {.val = 365}, {.val = 339}, {.val = 319}, {.val = 304}, {.val = 294}, {.val = 289}, {.val = 289}, {.val = 293}, {.val = 303}, {.val = 317}, {.val = 337}, {.val = 362}, {.val = 393}, {.val = 430}, {.val = 472}, {.val = 519}, {.val = 519}, {.val = 565}, {.val = 513}, {.val = 465}, {.val = 425}, {.val = 392}, {.val = 364}, {.val = 344}, {.val = 328}, {.val = 318}, {.val = 312}, {.val = 312}, {.val = 316}, {.val = 326}, {.val = 341}, {.val = 362}, {.val = 388}, {.val = 419}, {.val = 456}, {.val = 497}, {.val = 545}, {.val = 545}, {.val = 586}, {.val = 534}, {.val = 487}, {.val = 446}, {.val = 412}, {.val = 384}, {.val = 363}, {.val = 346}, {.val = 336}, {.val = 330}, {.val = 330}, {.val = 335}, {.val = 345}, {.val = 360}, {.val = 381}, {.val = 406}, {.val = 437}, {.val = 476}, {.val = 517}, {.val = 564}, {.val = 564}, {.val = 586}, {.val = 534}, {.val = 487}, {.val = 446}, {.val = 412}, {.val = 384}, {.val = 363}, {.val = 346}, {.val = 336}, {.val = 330}, {.val = 330}, {.val = 335}, {.val = 345}, {.val = 360}, {.val = 381}, {.val = 406}, {.val = 437}, {.val = 476}, {.val = 517}, {.val = 564}, {.val = 564}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_4015_config[] = {
    {.val = 516}, {.val = 473}, {.val = 435}, {.val = 405}, {.val = 379}, {.val = 357}, {.val = 341}, {.val = 330}, {.val = 328}, {.val = 323}, {.val = 315}, {.val = 319}, {.val = 326}, {.val = 339}, {.val = 355}, {.val = 374}, {.val = 398}, {.val = 428}, {.val = 460}, {.val = 498}, {.val = 498}, {.val = 493}, {.val = 450}, {.val = 413}, {.val = 382}, {.val = 355}, {.val = 334}, {.val = 317}, {.val = 307}, {.val = 303}, {.val = 300}, {.val = 293}, {.val = 296}, {.val = 304}, {.val = 314}, {.val = 331}, {.val = 351}, {.val = 376}, {.val = 403}, {.val = 438}, {.val = 476}, {.val = 476}, {.val = 475}, {.val = 432}, {.val = 394}, {.val = 364}, {.val = 337}, {.val = 316}, {.val = 300}, {.val = 288}, {.val = 283}, {.val = 279}, {.val = 278}, {.val = 280}, {.val = 286}, {.val = 297}, {.val = 313}, {.val = 333}, {.val = 358}, {.val = 387}, {.val = 420}, {.val = 458}, {.val = 458}, {.val = 462}, {.val = 420}, {.val = 383}, {.val = 351}, {.val = 325}, {.val = 303}, {.val = 288}, {.val = 277}, {.val = 270}, {.val = 266}, {.val = 266}, {.val = 269}, {.val = 275}, {.val = 285}, {.val = 301}, {.val = 320}, {.val = 345}, {.val = 374}, {.val = 408}, {.val = 446}, {.val = 446}, {.val = 455}, {.val = 412}, {.val = 375}, {.val = 343}, {.val = 317}, {.val = 296}, {.val = 280}, {.val = 270}, {.val = 262}, {.val = 259}, {.val = 259}, {.val = 262}, {.val = 268}, {.val = 278}, {.val = 293}, {.val = 313}, {.val = 337}, {.val = 367}, {.val = 400}, {.val = 438}, {.val = 438}, {.val = 452}, {.val = 409}, {.val = 372}, {.val = 340}, {.val = 314}, {.val = 293}, {.val = 277}, {.val = 267}, {.val = 259}, {.val = 256}, {.val = 256}, {.val = 258}, {.val = 264}, {.val = 275}, {.val = 290}, {.val = 310}, {.val = 335}, {.val = 364}, {.val = 398}, {.val = 435}, {.val = 435}, {.val = 453}, {.val = 410}, {.val = 372}, {.val = 341}, {.val = 314}, {.val = 294}, {.val = 277}, {.val = 267}, {.val = 259}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 265}, {.val = 275}, {.val = 291}, {.val = 311}, {.val = 336}, {.val = 365}, {.val = 398}, {.val = 436}, {.val = 436}, {.val = 459}, {.val = 416}, {.val = 378}, {.val = 346}, {.val = 319}, {.val = 298}, {.val = 281}, {.val = 270}, {.val = 262}, {.val = 259}, {.val = 259}, {.val = 262}, {.val = 269}, {.val = 279}, {.val = 296}, {.val = 316}, {.val = 341}, {.val = 372}, {.val = 404}, {.val = 442}, {.val = 442}, {.val = 477}, {.val = 433}, {.val = 394}, {.val = 362}, {.val = 334}, {.val = 312}, {.val = 294}, {.val = 283}, {.val = 275}, {.val = 271}, {.val = 271}, {.val = 274}, {.val = 282}, {.val = 293}, {.val = 309}, {.val = 330}, {.val = 357}, {.val = 387}, {.val = 420}, {.val = 457}, {.val = 457}, {.val = 499}, {.val = 454}, {.val = 414}, {.val = 381}, {.val = 353}, {.val = 330}, {.val = 311}, {.val = 299}, {.val = 290}, {.val = 287}, {.val = 287}, {.val = 290}, {.val = 298}, {.val = 310}, {.val = 326}, {.val = 348}, {.val = 375}, {.val = 404}, {.val = 439}, {.val = 475}, {.val = 475}, {.val = 526}, {.val = 480}, {.val = 440}, {.val = 405}, {.val = 376}, {.val = 352}, {.val = 333}, {.val = 320}, {.val = 312}, {.val = 308}, {.val = 308}, {.val = 311}, {.val = 319}, {.val = 332}, {.val = 349}, {.val = 370}, {.val = 398}, {.val = 429}, {.val = 463}, {.val = 500}, {.val = 500}, {.val = 542}, {.val = 499}, {.val = 458}, {.val = 424}, {.val = 393}, {.val = 369}, {.val = 350}, {.val = 337}, {.val = 328}, {.val = 325}, {.val = 325}, {.val = 328}, {.val = 336}, {.val = 349}, {.val = 366}, {.val = 388}, {.val = 414}, {.val = 446}, {.val = 482}, {.val = 517}, {.val = 517}, {.val = 542}, {.val = 499}, {.val = 458}, {.val = 424}, {.val = 393}, {.val = 369}, {.val = 350}, {.val = 337}, {.val = 328}, {.val = 325}, {.val = 325}, {.val = 328}, {.val = 336}, {.val = 349}, {.val = 366}, {.val = 388}, {.val = 414}, {.val = 446}, {.val = 482}, {.val = 517}, {.val = 517}
};

static const esp_ipa_acc_lsc_lut_t s_esp_ipa_acc_lsc_SC202CS_1280_x_720_config[] = {
    {
        .color_temp = 2410,
        .lsc = {
            .gain_r  = s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_2410_config,
            .gain_gr = s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_2410_config,
            .gain_gb = s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_2410_config,
            .gain_b  = s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_2410_config,
            .lsc_gain_array_size = 273
        },
    },
    {
        .color_temp = 4015,
        .lsc = {
            .gain_r  = s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_4015_config,
            .gain_gr = s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_4015_config,
            .gain_gb = s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_4015_config,
            .gain_b  = s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_4015_config,
            .lsc_gain_array_size = 273
        },
    },
};

static const esp_ipa_acc_lsc_t s_esp_ipa_acc_lsc_SC202CS_config[] = {
    {
        .width = 1280,
        .height = 720,
        .lsc_gain_table = s_esp_ipa_acc_lsc_SC202CS_1280_x_720_config,
        .lsc_gain_table_size = 2
    }
};

static const esp_ipa_acc_blc_param_t s_ipa_acc_blc_SC202CS_table[] = {
    {
        .gain = 1.000000,
        .blc_param = {
            .stretch = 0,
            .top_left_chan_offset = 16,
            .top_right_chan_offset = 16,
            .bottom_left_chan_offset = 16,
            .bottom_right_chan_offset = 16
        }
    },
};

static const esp_ipa_acc_blc_config_t s_ipa_acc_blc_SC202CS_config = {
    .model = 0,
    .blc_table = s_ipa_acc_blc_SC202CS_table,
    .blc_table_size = ARRAY_SIZE(s_ipa_acc_blc_SC202CS_table)
};

static const esp_ipa_acc_config_t s_ipa_acc_SC202CS_config = {
    .sat_table = s_ipa_acc_sat_SC202CS_config,
    .sat_table_size = ARRAY_SIZE(s_ipa_acc_sat_SC202CS_config),
    .ccm = &s_esp_ipa_acc_ccm_SC202CS_config,
    .lsc_table = s_esp_ipa_acc_lsc_SC202CS_config,
    .lsc_table_size = ARRAY_SIZE(s_esp_ipa_acc_lsc_SC202CS_config),
    .blc = &s_ipa_acc_blc_SC202CS_config,
};



static const char *s_ipa_SC202CS_names[] = {
    "esp_ipa_ian",
    "esp_ipa_awb",
    "esp_ipa_agc",
    "esp_ipa_adn",
    "esp_ipa_acc",
    "esp_ipa_aen",
};

static const esp_ipa_config_t s_ipa_SC202CS_config = {
    .names = s_ipa_SC202CS_names,
    .nums = ARRAY_SIZE(s_ipa_SC202CS_names),
    .version = 1,
    .ian = &s_ipa_ian_SC202CS_config,
    .awb = &s_ipa_awb_SC202CS_config,
    .agc = &s_ipa_agc_SC202CS_config,
    .adn = &s_ipa_adn_SC202CS_config,
    .acc = &s_ipa_acc_SC202CS_config,
    .aen = &s_ipa_aen_SC202CS_config,
};

static const esp_video_ipa_index_t s_video_ipa_configs[] = {
    {
        .name = "SC202CS",
        .ipa_config = &s_ipa_SC202CS_config
    },
};

const esp_ipa_config_t *esp_ipa_pipeline_get_config(const char *name)
{
    for (int i = 0; i < ARRAY_SIZE(s_video_ipa_configs); i++) {
        if (!strcmp(name, s_video_ipa_configs[i].name)) {
            return s_video_ipa_configs[i].ipa_config;
        }
    }
    return NULL;
}

/* Json file: C:\Users\djhui\OneDrive\Github\My-HA\components\m5_tab5_camera\sc202cs_default.json */

