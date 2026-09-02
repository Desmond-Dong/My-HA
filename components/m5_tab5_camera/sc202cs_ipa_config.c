/*
* SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
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
        .a0 = 0.8790322580645161,
        .a1 = 0.2903225806451613
    },
    {
        .a0 = 0.7906976744186046,
        .a1 = 0.3643410852713178
    },
    {
        .a0 = 0.7230769230769231,
        .a1 = 0.4153846153846154
    },
    {
        .a0 = 0.65625,
        .a1 = 0.4609375
    },
    {
        .a0 = 0.6209677419354839,
        .a1 = 0.5
    },
    {
        .a0 = 0.5846153846153846,
        .a1 = 0.5230769230769231
    },
    {
        .a0 = 0.5511811023622047,
        .a1 = 0.5433070866141733
    },
    {
        .a0 = 0.5267175572519084,
        .a1 = 0.5648854961832062
    },
    {
        .a0 = 0.5,
        .a1 = 0.5806451612903226
    },
    {
        .a0 = 0.4765625,
        .a1 = 0.59375
    },
    {
        .a0 = 0.45528455284552843,
        .a1 = 0.6016260162601627
    },
    {
        .a0 = 0.4435483870967742,
        .a1 = 0.6129032258064516
    },
    {
        .a0 = 0.42857142857142855,
        .a1 = 0.626984126984127
    },
    {
        .a0 = 0.4186046511627907,
        .a1 = 0.6356589147286822
    },
    {
        .a0 = 0.4076923076923077,
        .a1 = 0.6384615384615384
    },
    {
        .a0 = 0.38095238095238093,
        .a1 = 0.6587301587301587
    },
};

static const float s_esp_ipa_ian_ct_SC202CS_g_a2[] = {
    -237.7462258184258, 2177.004255235854, -5984.610095953817, 8062.352386114409, 
};

static const esp_ipa_ian_ct_config_t s_esp_ipa_ian_ct_SC202CS_config = {
    .model = 2,
    .m_a0 = -0.22916907819908985,
    .m_a1 = -0.4463635580822185,
    .m_a2 = 0.8595520375406922,
    .f_n0 = 0.0033,
    .bp = s_ipa_ian_ct_SC202CS_basic_param,
    .bp_nums = ARRAY_SIZE(s_ipa_ian_ct_SC202CS_basic_param),
    .min_step = 1,
    .g_a0 = -0.332,
    .g_a1 = -0.1858,
    .g_a2 = s_esp_ipa_ian_ct_SC202CS_g_a2,
    .g_a2_nums = ARRAY_SIZE(s_esp_ipa_ian_ct_SC202CS_g_a2)       
};

static const esp_ipa_ian_luma_ae_config_t s_esp_ipa_ian_luma_ae_SC202CS_config = {                 
    .weight = {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    },
};

static const float s_esp_ipa_ian_luma_env_speed_param_SC202CS_config[] = {
    -0.005463, -0.010018, 0.0, 0.033241, 0.085583, 0.136704, 0.160734, 0.148777, 0.148777, 0.160734, 0.136704, 0.085583, 0.033241, 0.0, -0.010018, -0.005463, 
};

static const esp_ipa_ian_luma_env_config_t s_esp_ipa_ian_luma_env_SC202CS_config = {
    .k = 250000,
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

static const esp_ipa_awb_config_t s_ipa_awb_SC202CS_config = {
    .model = ESP_IPA_AWB_MODEL_1,
    .min_counted = 1200,
    .min_red_gain_step = 0.0033,
    .min_blue_gain_step = 0.0033,
    .red_gain_scale = 1.000000f,
    .blue_gain_scale = 1.000000f,
    .range = {
        .green_max = 210,
        .green_min = 98,
        .rg_max = 0.879,
        .rg_min = 0.3801,
        .bg_max = 0.6587,
        .bg_min = 0.2903
    },
    .green_luma_env = "dummy_awb_luma",
    .green_luma_init = 200,
    .green_luma_step_ratio = 0.3,
    .enable_sub_win = false,
    .min_subwin_wp_counted = 0,
    .min_subwin_participated = 0,
    .subwin_weight = {
        { 1.000000f, 1.000000f, 1.000000f, 1.000000f, 1.000000f },
        { 1.000000f, 1.000000f, 1.000000f, 1.000000f, 1.000000f },
        { 1.000000f, 1.000000f, 1.000000f, 1.000000f, 1.000000f },
        { 1.000000f, 1.000000f, 1.000000f, 1.000000f, 1.000000f },
        { 1.000000f, 1.000000f, 1.000000f, 1.000000f, 1.000000f },
    },
    .subwin_green_dark = 0,
    .subwin_green_mid = 100,
    .subwin_green_bright = 200,
    .zones = NULL,
    .zones_count = 0,
    .ref_points = NULL,
    .ref_points_count = 0,
    .new_w = 0.300000f,
    .prev_w = 0.700000f,
    .export_ct = false,
    .outlier_rg = 0.000000f,
    .outlier_bg = 0.000000f,
    .zone_hysteresis_ratio = 0.000000f,
    .zone_switch_count = 0,
    .type_counter_max = 20000
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
    .min_gain_step = 0.03,
    .max_gain = 0,
    .inc_gain_ratio = 0.32,
    .dec_gain_ratio = 0.42,
    .anti_flicker_mode = ESP_IPA_AGC_ANTI_FLICKER_PART,
    .ac_freq = 50,
    .gain_only = false,
    .fixed_exposure_time = 0,
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
        .use_env_luma = false,
        .luma_high_threshold = 202,
        .luma_low_threshold = 119,
        .weight_offset = 5,
        .luma_offset = -3
    },
    .low_light_prior_config = {
        .use_env_luma = false,
        .luma_high_threshold = 56,
        .luma_low_threshold = 48,
        .weight_offset = 5,
        .luma_offset = 1
    },
    .light_threshold_config = {
        .use_env_luma = false,
        .table = s_ipa_agc_meter_light_thresholds_SC202CS,
        .table_size = ARRAY_SIZE(s_ipa_agc_meter_light_thresholds_SC202CS)
    },
};

static const esp_ipa_adn_bf_t s_ipa_adn_bf_SC202CS_config[] = {
    {
        .gain = 1000,
        .bf = {
            .level = 2,
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
            .level = 10,
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
            .gradient_ratio = 1.5
        }
    },
    {
        .gain = 4000,
        .dm = {
            .gradient_ratio = 1.25
        }
    },
    {
        .gain = 8000,
        .dm = {
            .gradient_ratio = 1.05
        }
    },
    {
        .gain = 12000,
        .dm = {
            .gradient_ratio = 1.0
        }
    },
};

static const esp_ipa_adn_config_t s_ipa_adn_SC202CS_config = {
    .bf_table = s_ipa_adn_bf_SC202CS_config,
    .bf_table_size = ARRAY_SIZE(s_ipa_adn_bf_SC202CS_config),
    .dm_table = s_ipa_adn_dm_SC202CS_config,
    .dm_table_size = ARRAY_SIZE(s_ipa_adn_dm_SC202CS_config),
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
                { 1.0, 0.0, 0.0 },
                { 0.0, 1.0, 0.0 },
                { 0.0, 0.0, 1.0 }
            }
        }
    },
    {
        .color_temp = 2292,
        .ccm = {
            .matrix = {
                { 2.3384, -0.0453, -1.2931 },
                { -0.7008, 2.0795, -0.3787 },
                { -0.7768, -2.7677, 4.5445 }
            }
        }
    },
    {
        .color_temp = 2517,
        .ccm = {
            .matrix = {
                { 2.3024, -0.2366, -1.0658 },
                { -0.6758, 2.0428, -0.3671 },
                { -0.4542, -1.9995, 3.4537 }
            }
        }
    },
    {
        .color_temp = 2780,
        .ccm = {
            .matrix = {
                { 2.3862, -0.2663, -1.1199 },
                { -0.6061, 2.0379, -0.4318 },
                { -0.3462, -1.5729, 2.919 }
            }
        }
    },
    {
        .color_temp = 3055,
        .ccm = {
            .matrix = {
                { 2.3647, -0.407, -0.9577 },
                { -0.5832, 1.9531, -0.3699 },
                { -0.2728, -1.3316, 2.6043 }
            }
        }
    },
    {
        .color_temp = 3473,
        .ccm = {
            .matrix = {
                { 2.1205, -0.7703, -0.3502 },
                { -0.5106, 1.7223, -0.2117 },
                { -0.4047, -0.81, 2.2147 }
            }
        }
    },
    {
        .color_temp = 3800,
        .ccm = {
            .matrix = {
                { 2.0445, -0.7286, -0.3159 },
                { -0.4934, 1.7135, -0.2201 },
                { -0.3837, -0.6795, 2.0632 }
            }
        }
    },
    {
        .color_temp = 4193,
        .ccm = {
            .matrix = {
                { 2.0841, -0.762, -0.3221 },
                { -0.4528, 1.671, -0.2181 },
                { -0.3583, -0.5935, 1.9518 }
            }
        }
    },
    {
        .color_temp = 4583,
        .ccm = {
            .matrix = {
                { 2.0503, -0.7524, -0.2979 },
                { -0.4294, 1.655, -0.2256 },
                { -0.3272, -0.5457, 1.8729 }
            }
        }
    },
    {
        .color_temp = 5040,
        .ccm = {
            .matrix = {
                { 2.0601, -0.7831, -0.277 },
                { -0.4029, 1.629, -0.2261 },
                { -0.3123, -0.5083, 1.8205 }
            }
        }
    },
    {
        .color_temp = 5090,
        .ccm = {
            .matrix = {
                { 2.0614, -0.7925, -0.269 },
                { -0.402, 1.6266, -0.2246 },
                { -0.3114, -0.5098, 1.8212 }
            }
        }
    },
    {
        .color_temp = 5210,
        .ccm = {
            .matrix = {
                { 2.0263, -0.7904, -0.2358 },
                { -0.3715, 1.5908, -0.2193 },
                { -0.3328, -0.4288, 1.7615 }
            }
        }
    },
    {
        .color_temp = 5476,
        .ccm = {
            .matrix = {
                { 2.0768, -0.826, -0.2508 },
                { -0.3668, 1.6268, -0.26 },
                { -0.2397, -0.556, 1.7958 }
            }
        }
    },
    {
        .color_temp = 5770,
        .ccm = {
            .matrix = {
                { 2.0123, -0.7757, -0.2366 },
                { -0.3645, 1.6287, -0.2641 },
                { -0.2727, -0.4673, 1.7399 }
            }
        }
    },
    {
        .color_temp = 6000,
        .ccm = {
            .matrix = {
                { 2.0239, -0.7721, -0.2518 },
                { -0.348, 1.6239, -0.2759 },
                { -0.2684, -0.4494, 1.7179 }
            }
        }
    },
    {
        .color_temp = 6554,
        .ccm = {
            .matrix = {
                { 1.9064, -0.6038, -0.3025 },
                { -0.3397, 1.6788, -0.3391 },
                { -0.2118, -0.4685, 1.6803 }
            }
        }
    },
    {
        .color_temp = 7020,
        .ccm = {
            .matrix = {
                { 2.0853, -0.8223, -0.263 },
                { -0.2765, 1.5826, -0.3061 },
                { -0.1083, -0.6144, 1.7227 }
            }
        }
    },
    {
        .color_temp = 7265,
        .ccm = {
            .matrix = {
                { 2.1339, -0.9093, -0.2246 },
                { -0.2675, 1.5687, -0.3012 },
                { -0.1089, -0.6049, 1.7138 }
            }
        }
    },
    {
        .color_temp = 12000,
        .ccm = {
            .matrix = {
                { 1.0, 0.0, 0.0 },
                { 0.0, 1.0, 0.0 },
                { 0.0, 0.0, 1.0 }
            }
        }
    },
};

static const esp_ipa_acc_ccm_gain_lut_t s_esp_ipa_acc_ccm_SC202CS_gain_lut[] = {
    {
        .gain = 1.0,
        .strength = 1.0
    },
    {
        .gain = 8.0,
        .strength = 0.5
    },
    {
        .gain = 16.0,
        .strength = 0.0
    },
};

static const esp_ipa_acc_ccm_config_t s_esp_ipa_acc_ccm_SC202CS_config = {
    .model = 0,
    .luma_env = "ae.luma.avg",
    .luma_low_threshold = 28,
    .luma_low_ccm = {
        .matrix = {
            { 1.0, 0.0, 0.0 },
            { 0.0, 1.0, 0.0 },
            { 0.0, 0.0, 1.0 }
        }
    }
    ,
    .ccm_table = s_esp_ipa_acc_ccm_SC202CS_table,
    .ccm_table_size = 19,
    .gain_lut_enable = 1,
    .gain_lut = s_esp_ipa_acc_ccm_SC202CS_gain_lut,
    .gain_lut_size = ARRAY_SIZE(s_esp_ipa_acc_ccm_SC202CS_gain_lut),
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
    {.val = 516}, {.val = 473}, {.val = 435}, {.val = 405}, {.val = 379}, {.val = 357}, {.val = 341}, {.val = 330}, {.val = 328}, {.val = 323}, {.val = 315}, {.val = 319}, {.val = 326}, {.val = 339}, {.val = 355}, {.val = 374}, {.val = 398}, {.val = 428}, {.val = 460}, {.val = 498}, {.val = 498}, {.val = 493}, {.val = 450}, {.val = 413}, {.val = 382}, {.val = 355}, {.val = 334}, {.val = 317}, {.val = 307}, {.val = 303}, {.val = 300}, {.val = 293}, {.val = 296}, {.val = 304}, {.val = 314}, {.val = 331}, {.val = 351}, {.val = 376}, {.val = 403}, {.val = 438}, {.val = 476}, {.val = 476}, {.val = 475}, {.val = 432}, {.val = 394}, {.val = 364}, {.val = 337}, {.val = 316}, {.val = 300}, {.val = 288}, {.val = 283}, {.val = 279}, {.val = 278}, {.val = 280}, {.val = 286}, {.val = 297}, {.val = 313}, {.val = 333}, {.val = 358}, {.val = 387}, {.val = 420}, {.val = 458}, {.val = 458}, {.val = 462}, {.val = 420}, {.val = 383}, {.val = 351}, {.val = 325}, {.val = 303}, {.val = 288}, {.val = 277}, {.val = 270}, {.val = 266}, {.val = 266}, {.val = 269}, {.val = 275}, {.val = 285}, {.val = 301}, {.val = 320}, {.val = 345}, {.val = 374}, {.val = 408}, {.val = 446}, {.val = 446}, {.val = 455}, {.val = 412}, {.val = 375}, {.val = 343}, {.val = 317}, {.val = 296}, {.val = 280}, {.val = 270}, {.val = 262}, {.val = 259}, {.val = 259}, {.val = 262}, {.val = 268}, {.val = 278}, {.val = 293}, {.val = 313}, {.val = 337}, {.val = 367}, {.val = 400}, {.val = 438}, {.val = 438}, {.val = 452}, {.val = 409}, {.val = 372}, {.val = 340}, {.val = 314}, {.val = 293}, {.val = 277}, {.val = 267}, {.val = 259}, {.val = 256}, {.val = 256}, {.val = 258}, {.val = 264}, {.val = 275}, {.val = 290}, {.val = 310}, {.val = 335}, {.val = 364}, {.val = 398}, {.val = 435}, {.val = 435}, {.val = 454}, {.val = 411}, {.val = 373}, {.val = 342}, {.val = 314}, {.val = 294}, {.val = 278}, {.val = 267}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 265}, {.val = 276}, {.val = 291}, {.val = 311}, {.val = 336}, {.val = 366}, {.val = 400}, {.val = 438}, {.val = 438}, {.val = 461}, {.val = 418}, {.val = 381}, {.val = 348}, {.val = 322}, {.val = 300}, {.val = 284}, {.val = 272}, {.val = 265}, {.val = 261}, {.val = 261}, {.val = 264}, {.val = 271}, {.val = 282}, {.val = 298}, {.val = 318}, {.val = 343}, {.val = 373}, {.val = 407}, {.val = 445}, {.val = 445}, {.val = 476}, {.val = 432}, {.val = 393}, {.val = 360}, {.val = 333}, {.val = 311}, {.val = 294}, {.val = 282}, {.val = 274}, {.val = 271}, {.val = 270}, {.val = 274}, {.val = 281}, {.val = 293}, {.val = 309}, {.val = 330}, {.val = 355}, {.val = 385}, {.val = 419}, {.val = 457}, {.val = 457}, {.val = 497}, {.val = 450}, {.val = 409}, {.val = 377}, {.val = 349}, {.val = 327}, {.val = 309}, {.val = 296}, {.val = 289}, {.val = 285}, {.val = 285}, {.val = 289}, {.val = 297}, {.val = 309}, {.val = 325}, {.val = 347}, {.val = 372}, {.val = 401}, {.val = 435}, {.val = 474}, {.val = 474}, {.val = 522}, {.val = 476}, {.val = 434}, {.val = 399}, {.val = 371}, {.val = 348}, {.val = 331}, {.val = 318}, {.val = 309}, {.val = 305}, {.val = 305}, {.val = 309}, {.val = 317}, {.val = 330}, {.val = 347}, {.val = 368}, {.val = 393}, {.val = 424}, {.val = 456}, {.val = 495}, {.val = 495}, {.val = 542}, {.val = 493}, {.val = 455}, {.val = 417}, {.val = 388}, {.val = 364}, {.val = 347}, {.val = 333}, {.val = 326}, {.val = 323}, {.val = 321}, {.val = 327}, {.val = 333}, {.val = 347}, {.val = 363}, {.val = 385}, {.val = 409}, {.val = 440}, {.val = 472}, {.val = 513}, {.val = 513}, {.val = 542}, {.val = 493}, {.val = 455}, {.val = 417}, {.val = 388}, {.val = 364}, {.val = 347}, {.val = 333}, {.val = 326}, {.val = 323}, {.val = 321}, {.val = 327}, {.val = 333}, {.val = 347}, {.val = 363}, {.val = 385}, {.val = 409}, {.val = 440}, {.val = 472}, {.val = 513}, {.val = 513}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_4637_config[] = {
    {.val = 757}, {.val = 666}, {.val = 589}, {.val = 522}, {.val = 469}, {.val = 426}, {.val = 395}, {.val = 374}, {.val = 366}, {.val = 358}, {.val = 352}, {.val = 360}, {.val = 375}, {.val = 402}, {.val = 435}, {.val = 478}, {.val = 534}, {.val = 602}, {.val = 678}, {.val = 766}, {.val = 766}, {.val = 703}, {.val = 614}, {.val = 538}, {.val = 475}, {.val = 424}, {.val = 385}, {.val = 354}, {.val = 335}, {.val = 328}, {.val = 322}, {.val = 315}, {.val = 322}, {.val = 336}, {.val = 359}, {.val = 390}, {.val = 433}, {.val = 486}, {.val = 551}, {.val = 626}, {.val = 713}, {.val = 713}, {.val = 658}, {.val = 573}, {.val = 500}, {.val = 440}, {.val = 391}, {.val = 353}, {.val = 325}, {.val = 305}, {.val = 295}, {.val = 289}, {.val = 288}, {.val = 295}, {.val = 308}, {.val = 329}, {.val = 358}, {.val = 399}, {.val = 449}, {.val = 511}, {.val = 586}, {.val = 671}, {.val = 671}, {.val = 628}, {.val = 544}, {.val = 472}, {.val = 413}, {.val = 367}, {.val = 331}, {.val = 305}, {.val = 287}, {.val = 275}, {.val = 270}, {.val = 270}, {.val = 276}, {.val = 288}, {.val = 308}, {.val = 336}, {.val = 375}, {.val = 424}, {.val = 485}, {.val = 558}, {.val = 642}, {.val = 642}, {.val = 610}, {.val = 526}, {.val = 457}, {.val = 399}, {.val = 353}, {.val = 318}, {.val = 292}, {.val = 275}, {.val = 264}, {.val = 259}, {.val = 259}, {.val = 264}, {.val = 276}, {.val = 296}, {.val = 323}, {.val = 360}, {.val = 409}, {.val = 469}, {.val = 540}, {.val = 625}, {.val = 625}, {.val = 601}, {.val = 519}, {.val = 450}, {.val = 393}, {.val = 348}, {.val = 313}, {.val = 288}, {.val = 270}, {.val = 259}, {.val = 254}, {.val = 255}, {.val = 260}, {.val = 272}, {.val = 291}, {.val = 318}, {.val = 355}, {.val = 404}, {.val = 462}, {.val = 534}, {.val = 616}, {.val = 616}, {.val = 605}, {.val = 522}, {.val = 452}, {.val = 395}, {.val = 350}, {.val = 315}, {.val = 289}, {.val = 272}, {.val = 261}, {.val = 255}, {.val = 256}, {.val = 262}, {.val = 273}, {.val = 292}, {.val = 321}, {.val = 357}, {.val = 406}, {.val = 466}, {.val = 536}, {.val = 619}, {.val = 619}, {.val = 615}, {.val = 534}, {.val = 464}, {.val = 406}, {.val = 360}, {.val = 324}, {.val = 298}, {.val = 280}, {.val = 268}, {.val = 263}, {.val = 263}, {.val = 269}, {.val = 282}, {.val = 302}, {.val = 331}, {.val = 369}, {.val = 418}, {.val = 478}, {.val = 551}, {.val = 633}, {.val = 633}, {.val = 642}, {.val = 557}, {.val = 485}, {.val = 427}, {.val = 380}, {.val = 343}, {.val = 315}, {.val = 295}, {.val = 283}, {.val = 278}, {.val = 278}, {.val = 285}, {.val = 298}, {.val = 319}, {.val = 350}, {.val = 389}, {.val = 440}, {.val = 501}, {.val = 574}, {.val = 658}, {.val = 658}, {.val = 677}, {.val = 593}, {.val = 519}, {.val = 458}, {.val = 409}, {.val = 370}, {.val = 341}, {.val = 320}, {.val = 308}, {.val = 301}, {.val = 301}, {.val = 309}, {.val = 324}, {.val = 346}, {.val = 378}, {.val = 420}, {.val = 472}, {.val = 535}, {.val = 610}, {.val = 695}, {.val = 695}, {.val = 726}, {.val = 639}, {.val = 563}, {.val = 499}, {.val = 448}, {.val = 408}, {.val = 377}, {.val = 355}, {.val = 341}, {.val = 335}, {.val = 336}, {.val = 344}, {.val = 359}, {.val = 383}, {.val = 417}, {.val = 460}, {.val = 515}, {.val = 581}, {.val = 656}, {.val = 743}, {.val = 743}, {.val = 760}, {.val = 675}, {.val = 597}, {.val = 533}, {.val = 479}, {.val = 436}, {.val = 405}, {.val = 382}, {.val = 369}, {.val = 361}, {.val = 361}, {.val = 371}, {.val = 387}, {.val = 413}, {.val = 447}, {.val = 491}, {.val = 546}, {.val = 615}, {.val = 691}, {.val = 775}, {.val = 775}, {.val = 760}, {.val = 675}, {.val = 597}, {.val = 533}, {.val = 479}, {.val = 436}, {.val = 405}, {.val = 382}, {.val = 369}, {.val = 361}, {.val = 361}, {.val = 371}, {.val = 387}, {.val = 413}, {.val = 447}, {.val = 491}, {.val = 546}, {.val = 615}, {.val = 691}, {.val = 775}, {.val = 775}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_4637_config[] = {
    {.val = 569}, {.val = 518}, {.val = 473}, {.val = 434}, {.val = 403}, {.val = 376}, {.val = 357}, {.val = 343}, {.val = 339}, {.val = 334}, {.val = 327}, {.val = 331}, {.val = 340}, {.val = 356}, {.val = 375}, {.val = 399}, {.val = 429}, {.val = 465}, {.val = 507}, {.val = 554}, {.val = 554}, {.val = 538}, {.val = 487}, {.val = 442}, {.val = 405}, {.val = 374}, {.val = 349}, {.val = 329}, {.val = 317}, {.val = 312}, {.val = 308}, {.val = 301}, {.val = 304}, {.val = 314}, {.val = 328}, {.val = 346}, {.val = 371}, {.val = 401}, {.val = 437}, {.val = 478}, {.val = 525}, {.val = 525}, {.val = 513}, {.val = 462}, {.val = 418}, {.val = 382}, {.val = 351}, {.val = 327}, {.val = 309}, {.val = 294}, {.val = 288}, {.val = 283}, {.val = 282}, {.val = 286}, {.val = 293}, {.val = 307}, {.val = 325}, {.val = 348}, {.val = 378}, {.val = 414}, {.val = 455}, {.val = 503}, {.val = 503}, {.val = 494}, {.val = 444}, {.val = 401}, {.val = 364}, {.val = 335}, {.val = 312}, {.val = 293}, {.val = 281}, {.val = 273}, {.val = 269}, {.val = 268}, {.val = 272}, {.val = 279}, {.val = 292}, {.val = 309}, {.val = 333}, {.val = 363}, {.val = 398}, {.val = 438}, {.val = 486}, {.val = 486}, {.val = 482}, {.val = 433}, {.val = 390}, {.val = 354}, {.val = 324}, {.val = 302}, {.val = 284}, {.val = 271}, {.val = 264}, {.val = 260}, {.val = 260}, {.val = 263}, {.val = 270}, {.val = 282}, {.val = 300}, {.val = 323}, {.val = 352}, {.val = 387}, {.val = 428}, {.val = 475}, {.val = 475}, {.val = 476}, {.val = 427}, {.val = 384}, {.val = 349}, {.val = 320}, {.val = 297}, {.val = 279}, {.val = 267}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 266}, {.val = 278}, {.val = 295}, {.val = 319}, {.val = 348}, {.val = 383}, {.val = 424}, {.val = 471}, {.val = 471}, {.val = 478}, {.val = 428}, {.val = 385}, {.val = 350}, {.val = 320}, {.val = 298}, {.val = 280}, {.val = 268}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 267}, {.val = 279}, {.val = 297}, {.val = 320}, {.val = 349}, {.val = 385}, {.val = 426}, {.val = 472}, {.val = 472}, {.val = 484}, {.val = 435}, {.val = 392}, {.val = 357}, {.val = 327}, {.val = 304}, {.val = 286}, {.val = 273}, {.val = 265}, {.val = 261}, {.val = 261}, {.val = 265}, {.val = 273}, {.val = 286}, {.val = 304}, {.val = 327}, {.val = 357}, {.val = 392}, {.val = 433}, {.val = 480}, {.val = 480}, {.val = 497}, {.val = 448}, {.val = 405}, {.val = 369}, {.val = 339}, {.val = 316}, {.val = 296}, {.val = 284}, {.val = 275}, {.val = 271}, {.val = 271}, {.val = 275}, {.val = 284}, {.val = 298}, {.val = 316}, {.val = 340}, {.val = 370}, {.val = 405}, {.val = 447}, {.val = 494}, {.val = 494}, {.val = 516}, {.val = 467}, {.val = 424}, {.val = 387}, {.val = 357}, {.val = 332}, {.val = 313}, {.val = 300}, {.val = 291}, {.val = 287}, {.val = 287}, {.val = 291}, {.val = 301}, {.val = 314}, {.val = 334}, {.val = 359}, {.val = 389}, {.val = 425}, {.val = 465}, {.val = 513}, {.val = 513}, {.val = 541}, {.val = 492}, {.val = 448}, {.val = 412}, {.val = 380}, {.val = 356}, {.val = 336}, {.val = 322}, {.val = 313}, {.val = 308}, {.val = 309}, {.val = 313}, {.val = 323}, {.val = 337}, {.val = 358}, {.val = 382}, {.val = 413}, {.val = 449}, {.val = 490}, {.val = 538}, {.val = 538}, {.val = 562}, {.val = 511}, {.val = 467}, {.val = 431}, {.val = 399}, {.val = 373}, {.val = 353}, {.val = 339}, {.val = 330}, {.val = 326}, {.val = 326}, {.val = 331}, {.val = 341}, {.val = 356}, {.val = 376}, {.val = 400}, {.val = 432}, {.val = 467}, {.val = 509}, {.val = 556}, {.val = 556}, {.val = 562}, {.val = 511}, {.val = 467}, {.val = 431}, {.val = 399}, {.val = 373}, {.val = 353}, {.val = 339}, {.val = 330}, {.val = 326}, {.val = 326}, {.val = 331}, {.val = 341}, {.val = 356}, {.val = 376}, {.val = 400}, {.val = 432}, {.val = 467}, {.val = 509}, {.val = 556}, {.val = 556}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_4637_config[] = {
    {.val = 557}, {.val = 507}, {.val = 464}, {.val = 426}, {.val = 395}, {.val = 369}, {.val = 350}, {.val = 337}, {.val = 334}, {.val = 328}, {.val = 321}, {.val = 325}, {.val = 335}, {.val = 351}, {.val = 370}, {.val = 394}, {.val = 424}, {.val = 459}, {.val = 500}, {.val = 546}, {.val = 546}, {.val = 529}, {.val = 479}, {.val = 436}, {.val = 399}, {.val = 369}, {.val = 344}, {.val = 325}, {.val = 313}, {.val = 307}, {.val = 303}, {.val = 297}, {.val = 301}, {.val = 310}, {.val = 324}, {.val = 343}, {.val = 367}, {.val = 397}, {.val = 432}, {.val = 474}, {.val = 519}, {.val = 519}, {.val = 508}, {.val = 459}, {.val = 415}, {.val = 380}, {.val = 349}, {.val = 324}, {.val = 306}, {.val = 292}, {.val = 285}, {.val = 280}, {.val = 279}, {.val = 282}, {.val = 291}, {.val = 304}, {.val = 322}, {.val = 346}, {.val = 376}, {.val = 411}, {.val = 451}, {.val = 498}, {.val = 498}, {.val = 494}, {.val = 444}, {.val = 400}, {.val = 365}, {.val = 335}, {.val = 311}, {.val = 292}, {.val = 279}, {.val = 271}, {.val = 267}, {.val = 266}, {.val = 270}, {.val = 277}, {.val = 290}, {.val = 308}, {.val = 332}, {.val = 361}, {.val = 396}, {.val = 436}, {.val = 482}, {.val = 482}, {.val = 484}, {.val = 435}, {.val = 392}, {.val = 356}, {.val = 325}, {.val = 302}, {.val = 284}, {.val = 271}, {.val = 263}, {.val = 259}, {.val = 259}, {.val = 262}, {.val = 269}, {.val = 282}, {.val = 299}, {.val = 323}, {.val = 352}, {.val = 387}, {.val = 427}, {.val = 474}, {.val = 474}, {.val = 482}, {.val = 433}, {.val = 389}, {.val = 352}, {.val = 322}, {.val = 299}, {.val = 281}, {.val = 268}, {.val = 260}, {.val = 256}, {.val = 255}, {.val = 258}, {.val = 266}, {.val = 278}, {.val = 296}, {.val = 319}, {.val = 348}, {.val = 384}, {.val = 423}, {.val = 469}, {.val = 469}, {.val = 485}, {.val = 435}, {.val = 392}, {.val = 355}, {.val = 324}, {.val = 301}, {.val = 283}, {.val = 269}, {.val = 261}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 267}, {.val = 279}, {.val = 297}, {.val = 321}, {.val = 350}, {.val = 385}, {.val = 426}, {.val = 471}, {.val = 471}, {.val = 494}, {.val = 445}, {.val = 400}, {.val = 363}, {.val = 333}, {.val = 308}, {.val = 289}, {.val = 275}, {.val = 266}, {.val = 262}, {.val = 261}, {.val = 265}, {.val = 273}, {.val = 286}, {.val = 304}, {.val = 329}, {.val = 358}, {.val = 394}, {.val = 435}, {.val = 480}, {.val = 480}, {.val = 510}, {.val = 460}, {.val = 415}, {.val = 377}, {.val = 346}, {.val = 320}, {.val = 300}, {.val = 287}, {.val = 277}, {.val = 273}, {.val = 272}, {.val = 276}, {.val = 285}, {.val = 298}, {.val = 317}, {.val = 342}, {.val = 372}, {.val = 408}, {.val = 448}, {.val = 495}, {.val = 495}, {.val = 532}, {.val = 481}, {.val = 436}, {.val = 397}, {.val = 366}, {.val = 339}, {.val = 319}, {.val = 305}, {.val = 294}, {.val = 289}, {.val = 288}, {.val = 293}, {.val = 303}, {.val = 316}, {.val = 336}, {.val = 361}, {.val = 392}, {.val = 427}, {.val = 468}, {.val = 515}, {.val = 515}, {.val = 560}, {.val = 509}, {.val = 464}, {.val = 424}, {.val = 391}, {.val = 364}, {.val = 343}, {.val = 328}, {.val = 318}, {.val = 312}, {.val = 312}, {.val = 316}, {.val = 326}, {.val = 341}, {.val = 361}, {.val = 386}, {.val = 416}, {.val = 453}, {.val = 494}, {.val = 540}, {.val = 540}, {.val = 583}, {.val = 530}, {.val = 484}, {.val = 444}, {.val = 411}, {.val = 383}, {.val = 361}, {.val = 347}, {.val = 335}, {.val = 330}, {.val = 330}, {.val = 334}, {.val = 343}, {.val = 359}, {.val = 379}, {.val = 405}, {.val = 435}, {.val = 473}, {.val = 513}, {.val = 559}, {.val = 559}, {.val = 583}, {.val = 530}, {.val = 484}, {.val = 444}, {.val = 411}, {.val = 383}, {.val = 361}, {.val = 347}, {.val = 335}, {.val = 330}, {.val = 330}, {.val = 334}, {.val = 343}, {.val = 359}, {.val = 379}, {.val = 405}, {.val = 435}, {.val = 473}, {.val = 513}, {.val = 559}, {.val = 559}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_4637_config[] = {
    {.val = 514}, {.val = 471}, {.val = 434}, {.val = 404}, {.val = 377}, {.val = 356}, {.val = 339}, {.val = 328}, {.val = 327}, {.val = 321}, {.val = 315}, {.val = 317}, {.val = 325}, {.val = 337}, {.val = 353}, {.val = 371}, {.val = 396}, {.val = 424}, {.val = 458}, {.val = 493}, {.val = 493}, {.val = 491}, {.val = 448}, {.val = 410}, {.val = 380}, {.val = 354}, {.val = 333}, {.val = 317}, {.val = 307}, {.val = 303}, {.val = 299}, {.val = 293}, {.val = 296}, {.val = 303}, {.val = 314}, {.val = 329}, {.val = 350}, {.val = 374}, {.val = 402}, {.val = 434}, {.val = 471}, {.val = 471}, {.val = 473}, {.val = 430}, {.val = 393}, {.val = 363}, {.val = 337}, {.val = 315}, {.val = 300}, {.val = 288}, {.val = 282}, {.val = 279}, {.val = 276}, {.val = 280}, {.val = 286}, {.val = 296}, {.val = 312}, {.val = 331}, {.val = 356}, {.val = 384}, {.val = 417}, {.val = 454}, {.val = 454}, {.val = 461}, {.val = 419}, {.val = 381}, {.val = 350}, {.val = 324}, {.val = 302}, {.val = 288}, {.val = 277}, {.val = 269}, {.val = 266}, {.val = 265}, {.val = 269}, {.val = 274}, {.val = 285}, {.val = 299}, {.val = 319}, {.val = 344}, {.val = 372}, {.val = 405}, {.val = 443}, {.val = 443}, {.val = 454}, {.val = 411}, {.val = 373}, {.val = 342}, {.val = 316}, {.val = 296}, {.val = 280}, {.val = 269}, {.val = 262}, {.val = 259}, {.val = 259}, {.val = 262}, {.val = 267}, {.val = 277}, {.val = 292}, {.val = 312}, {.val = 336}, {.val = 364}, {.val = 398}, {.val = 434}, {.val = 434}, {.val = 451}, {.val = 408}, {.val = 370}, {.val = 340}, {.val = 313}, {.val = 293}, {.val = 277}, {.val = 266}, {.val = 259}, {.val = 256}, {.val = 256}, {.val = 258}, {.val = 264}, {.val = 274}, {.val = 289}, {.val = 308}, {.val = 333}, {.val = 362}, {.val = 395}, {.val = 432}, {.val = 432}, {.val = 452}, {.val = 410}, {.val = 373}, {.val = 341}, {.val = 315}, {.val = 294}, {.val = 278}, {.val = 267}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 265}, {.val = 275}, {.val = 290}, {.val = 309}, {.val = 335}, {.val = 364}, {.val = 397}, {.val = 434}, {.val = 434}, {.val = 461}, {.val = 417}, {.val = 380}, {.val = 348}, {.val = 322}, {.val = 300}, {.val = 283}, {.val = 272}, {.val = 265}, {.val = 261}, {.val = 261}, {.val = 264}, {.val = 270}, {.val = 281}, {.val = 297}, {.val = 317}, {.val = 342}, {.val = 370}, {.val = 404}, {.val = 442}, {.val = 442}, {.val = 475}, {.val = 430}, {.val = 392}, {.val = 360}, {.val = 333}, {.val = 311}, {.val = 294}, {.val = 283}, {.val = 274}, {.val = 271}, {.val = 270}, {.val = 273}, {.val = 280}, {.val = 292}, {.val = 308}, {.val = 328}, {.val = 353}, {.val = 383}, {.val = 416}, {.val = 454}, {.val = 454}, {.val = 495}, {.val = 449}, {.val = 410}, {.val = 376}, {.val = 349}, {.val = 327}, {.val = 310}, {.val = 297}, {.val = 289}, {.val = 285}, {.val = 285}, {.val = 288}, {.val = 296}, {.val = 308}, {.val = 324}, {.val = 345}, {.val = 369}, {.val = 398}, {.val = 433}, {.val = 470}, {.val = 470}, {.val = 521}, {.val = 473}, {.val = 432}, {.val = 399}, {.val = 371}, {.val = 348}, {.val = 331}, {.val = 318}, {.val = 309}, {.val = 305}, {.val = 305}, {.val = 309}, {.val = 317}, {.val = 329}, {.val = 346}, {.val = 366}, {.val = 392}, {.val = 421}, {.val = 454}, {.val = 491}, {.val = 491}, {.val = 539}, {.val = 493}, {.val = 451}, {.val = 416}, {.val = 386}, {.val = 364}, {.val = 346}, {.val = 333}, {.val = 326}, {.val = 321}, {.val = 320}, {.val = 325}, {.val = 332}, {.val = 344}, {.val = 360}, {.val = 382}, {.val = 408}, {.val = 437}, {.val = 469}, {.val = 507}, {.val = 507}, {.val = 539}, {.val = 493}, {.val = 451}, {.val = 416}, {.val = 386}, {.val = 364}, {.val = 346}, {.val = 333}, {.val = 326}, {.val = 321}, {.val = 320}, {.val = 325}, {.val = 332}, {.val = 344}, {.val = 360}, {.val = 382}, {.val = 408}, {.val = 437}, {.val = 469}, {.val = 507}, {.val = 507}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_5210_config[] = {
    {.val = 742}, {.val = 656}, {.val = 582}, {.val = 517}, {.val = 465}, {.val = 423}, {.val = 393}, {.val = 372}, {.val = 364}, {.val = 357}, {.val = 351}, {.val = 359}, {.val = 373}, {.val = 399}, {.val = 432}, {.val = 473}, {.val = 527}, {.val = 591}, {.val = 666}, {.val = 749}, {.val = 749}, {.val = 687}, {.val = 605}, {.val = 530}, {.val = 471}, {.val = 421}, {.val = 382}, {.val = 353}, {.val = 334}, {.val = 327}, {.val = 321}, {.val = 314}, {.val = 321}, {.val = 335}, {.val = 357}, {.val = 388}, {.val = 429}, {.val = 481}, {.val = 542}, {.val = 615}, {.val = 697}, {.val = 697}, {.val = 648}, {.val = 565}, {.val = 494}, {.val = 436}, {.val = 389}, {.val = 351}, {.val = 324}, {.val = 305}, {.val = 295}, {.val = 289}, {.val = 288}, {.val = 295}, {.val = 308}, {.val = 328}, {.val = 356}, {.val = 395}, {.val = 444}, {.val = 505}, {.val = 576}, {.val = 656}, {.val = 656}, {.val = 618}, {.val = 537}, {.val = 468}, {.val = 411}, {.val = 365}, {.val = 330}, {.val = 305}, {.val = 286}, {.val = 275}, {.val = 271}, {.val = 270}, {.val = 276}, {.val = 288}, {.val = 307}, {.val = 336}, {.val = 372}, {.val = 421}, {.val = 479}, {.val = 551}, {.val = 629}, {.val = 629}, {.val = 600}, {.val = 519}, {.val = 452}, {.val = 396}, {.val = 351}, {.val = 318}, {.val = 292}, {.val = 275}, {.val = 264}, {.val = 259}, {.val = 260}, {.val = 265}, {.val = 276}, {.val = 295}, {.val = 322}, {.val = 359}, {.val = 406}, {.val = 465}, {.val = 532}, {.val = 613}, {.val = 613}, {.val = 591}, {.val = 513}, {.val = 445}, {.val = 389}, {.val = 346}, {.val = 312}, {.val = 287}, {.val = 270}, {.val = 260}, {.val = 254}, {.val = 255}, {.val = 260}, {.val = 271}, {.val = 290}, {.val = 317}, {.val = 353}, {.val = 400}, {.val = 457}, {.val = 526}, {.val = 605}, {.val = 605}, {.val = 594}, {.val = 515}, {.val = 447}, {.val = 392}, {.val = 349}, {.val = 314}, {.val = 289}, {.val = 271}, {.val = 260}, {.val = 255}, {.val = 256}, {.val = 262}, {.val = 273}, {.val = 292}, {.val = 319}, {.val = 355}, {.val = 402}, {.val = 460}, {.val = 528}, {.val = 607}, {.val = 607}, {.val = 607}, {.val = 527}, {.val = 459}, {.val = 403}, {.val = 358}, {.val = 324}, {.val = 298}, {.val = 279}, {.val = 269}, {.val = 263}, {.val = 263}, {.val = 269}, {.val = 282}, {.val = 300}, {.val = 329}, {.val = 367}, {.val = 414}, {.val = 472}, {.val = 543}, {.val = 621}, {.val = 621}, {.val = 629}, {.val = 549}, {.val = 481}, {.val = 423}, {.val = 377}, {.val = 341}, {.val = 314}, {.val = 295}, {.val = 283}, {.val = 277}, {.val = 278}, {.val = 284}, {.val = 298}, {.val = 319}, {.val = 347}, {.val = 386}, {.val = 435}, {.val = 494}, {.val = 565}, {.val = 644}, {.val = 644}, {.val = 665}, {.val = 583}, {.val = 512}, {.val = 453}, {.val = 406}, {.val = 367}, {.val = 340}, {.val = 320}, {.val = 307}, {.val = 301}, {.val = 301}, {.val = 308}, {.val = 323}, {.val = 345}, {.val = 375}, {.val = 416}, {.val = 465}, {.val = 528}, {.val = 598}, {.val = 679}, {.val = 679}, {.val = 709}, {.val = 628}, {.val = 555}, {.val = 495}, {.val = 444}, {.val = 404}, {.val = 374}, {.val = 353}, {.val = 340}, {.val = 333}, {.val = 334}, {.val = 343}, {.val = 357}, {.val = 381}, {.val = 413}, {.val = 455}, {.val = 507}, {.val = 570}, {.val = 643}, {.val = 725}, {.val = 725}, {.val = 746}, {.val = 660}, {.val = 590}, {.val = 524}, {.val = 474}, {.val = 433}, {.val = 401}, {.val = 381}, {.val = 366}, {.val = 360}, {.val = 360}, {.val = 369}, {.val = 384}, {.val = 409}, {.val = 442}, {.val = 488}, {.val = 540}, {.val = 602}, {.val = 678}, {.val = 759}, {.val = 759}, {.val = 746}, {.val = 660}, {.val = 590}, {.val = 524}, {.val = 474}, {.val = 433}, {.val = 401}, {.val = 381}, {.val = 366}, {.val = 360}, {.val = 360}, {.val = 369}, {.val = 384}, {.val = 409}, {.val = 442}, {.val = 488}, {.val = 540}, {.val = 602}, {.val = 678}, {.val = 759}, {.val = 759}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_5210_config[] = {
    {.val = 566}, {.val = 516}, {.val = 471}, {.val = 433}, {.val = 401}, {.val = 376}, {.val = 356}, {.val = 342}, {.val = 338}, {.val = 333}, {.val = 326}, {.val = 330}, {.val = 339}, {.val = 354}, {.val = 374}, {.val = 397}, {.val = 427}, {.val = 463}, {.val = 504}, {.val = 549}, {.val = 549}, {.val = 535}, {.val = 484}, {.val = 441}, {.val = 404}, {.val = 373}, {.val = 348}, {.val = 329}, {.val = 316}, {.val = 312}, {.val = 307}, {.val = 300}, {.val = 304}, {.val = 313}, {.val = 326}, {.val = 345}, {.val = 369}, {.val = 399}, {.val = 435}, {.val = 476}, {.val = 521}, {.val = 521}, {.val = 510}, {.val = 460}, {.val = 417}, {.val = 380}, {.val = 351}, {.val = 326}, {.val = 308}, {.val = 294}, {.val = 288}, {.val = 283}, {.val = 282}, {.val = 285}, {.val = 293}, {.val = 306}, {.val = 324}, {.val = 348}, {.val = 378}, {.val = 413}, {.val = 453}, {.val = 499}, {.val = 499}, {.val = 492}, {.val = 442}, {.val = 399}, {.val = 364}, {.val = 335}, {.val = 311}, {.val = 293}, {.val = 280}, {.val = 273}, {.val = 269}, {.val = 268}, {.val = 272}, {.val = 279}, {.val = 292}, {.val = 309}, {.val = 332}, {.val = 361}, {.val = 396}, {.val = 437}, {.val = 483}, {.val = 483}, {.val = 480}, {.val = 431}, {.val = 389}, {.val = 353}, {.val = 324}, {.val = 301}, {.val = 284}, {.val = 271}, {.val = 264}, {.val = 260}, {.val = 260}, {.val = 263}, {.val = 270}, {.val = 282}, {.val = 299}, {.val = 323}, {.val = 351}, {.val = 386}, {.val = 426}, {.val = 473}, {.val = 473}, {.val = 475}, {.val = 426}, {.val = 384}, {.val = 348}, {.val = 319}, {.val = 296}, {.val = 280}, {.val = 267}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 266}, {.val = 278}, {.val = 295}, {.val = 319}, {.val = 347}, {.val = 382}, {.val = 422}, {.val = 468}, {.val = 468}, {.val = 475}, {.val = 426}, {.val = 385}, {.val = 349}, {.val = 320}, {.val = 297}, {.val = 280}, {.val = 268}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 267}, {.val = 279}, {.val = 296}, {.val = 319}, {.val = 348}, {.val = 383}, {.val = 424}, {.val = 469}, {.val = 469}, {.val = 481}, {.val = 433}, {.val = 391}, {.val = 356}, {.val = 326}, {.val = 304}, {.val = 286}, {.val = 273}, {.val = 265}, {.val = 261}, {.val = 261}, {.val = 264}, {.val = 273}, {.val = 285}, {.val = 303}, {.val = 327}, {.val = 356}, {.val = 391}, {.val = 431}, {.val = 477}, {.val = 477}, {.val = 494}, {.val = 446}, {.val = 403}, {.val = 368}, {.val = 338}, {.val = 315}, {.val = 297}, {.val = 283}, {.val = 275}, {.val = 271}, {.val = 271}, {.val = 275}, {.val = 283}, {.val = 297}, {.val = 315}, {.val = 339}, {.val = 368}, {.val = 404}, {.val = 444}, {.val = 491}, {.val = 491}, {.val = 512}, {.val = 464}, {.val = 423}, {.val = 386}, {.val = 356}, {.val = 332}, {.val = 313}, {.val = 300}, {.val = 290}, {.val = 286}, {.val = 286}, {.val = 291}, {.val = 300}, {.val = 314}, {.val = 333}, {.val = 357}, {.val = 387}, {.val = 422}, {.val = 463}, {.val = 509}, {.val = 509}, {.val = 538}, {.val = 489}, {.val = 446}, {.val = 410}, {.val = 379}, {.val = 355}, {.val = 335}, {.val = 322}, {.val = 313}, {.val = 308}, {.val = 307}, {.val = 313}, {.val = 322}, {.val = 337}, {.val = 356}, {.val = 381}, {.val = 412}, {.val = 447}, {.val = 488}, {.val = 534}, {.val = 534}, {.val = 558}, {.val = 508}, {.val = 466}, {.val = 428}, {.val = 397}, {.val = 371}, {.val = 353}, {.val = 339}, {.val = 330}, {.val = 325}, {.val = 325}, {.val = 330}, {.val = 339}, {.val = 354}, {.val = 375}, {.val = 399}, {.val = 429}, {.val = 464}, {.val = 505}, {.val = 552}, {.val = 552}, {.val = 558}, {.val = 508}, {.val = 466}, {.val = 428}, {.val = 397}, {.val = 371}, {.val = 353}, {.val = 339}, {.val = 330}, {.val = 325}, {.val = 325}, {.val = 330}, {.val = 339}, {.val = 354}, {.val = 375}, {.val = 399}, {.val = 429}, {.val = 464}, {.val = 505}, {.val = 552}, {.val = 552}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_5210_config[] = {
    {.val = 553}, {.val = 505}, {.val = 461}, {.val = 425}, {.val = 394}, {.val = 369}, {.val = 350}, {.val = 336}, {.val = 333}, {.val = 328}, {.val = 321}, {.val = 325}, {.val = 335}, {.val = 350}, {.val = 369}, {.val = 392}, {.val = 421}, {.val = 456}, {.val = 497}, {.val = 542}, {.val = 542}, {.val = 526}, {.val = 477}, {.val = 435}, {.val = 398}, {.val = 368}, {.val = 343}, {.val = 325}, {.val = 312}, {.val = 308}, {.val = 303}, {.val = 297}, {.val = 301}, {.val = 310}, {.val = 323}, {.val = 342}, {.val = 366}, {.val = 395}, {.val = 430}, {.val = 470}, {.val = 514}, {.val = 514}, {.val = 505}, {.val = 457}, {.val = 414}, {.val = 378}, {.val = 348}, {.val = 324}, {.val = 306}, {.val = 292}, {.val = 285}, {.val = 280}, {.val = 279}, {.val = 283}, {.val = 291}, {.val = 304}, {.val = 322}, {.val = 345}, {.val = 375}, {.val = 408}, {.val = 448}, {.val = 494}, {.val = 494}, {.val = 490}, {.val = 442}, {.val = 399}, {.val = 363}, {.val = 334}, {.val = 310}, {.val = 292}, {.val = 280}, {.val = 271}, {.val = 267}, {.val = 267}, {.val = 270}, {.val = 277}, {.val = 290}, {.val = 308}, {.val = 331}, {.val = 360}, {.val = 394}, {.val = 433}, {.val = 480}, {.val = 480}, {.val = 482}, {.val = 434}, {.val = 390}, {.val = 355}, {.val = 325}, {.val = 302}, {.val = 284}, {.val = 271}, {.val = 264}, {.val = 260}, {.val = 259}, {.val = 262}, {.val = 269}, {.val = 281}, {.val = 299}, {.val = 322}, {.val = 351}, {.val = 385}, {.val = 425}, {.val = 469}, {.val = 469}, {.val = 479}, {.val = 430}, {.val = 388}, {.val = 352}, {.val = 322}, {.val = 299}, {.val = 281}, {.val = 268}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 258}, {.val = 266}, {.val = 278}, {.val = 295}, {.val = 318}, {.val = 347}, {.val = 381}, {.val = 421}, {.val = 466}, {.val = 466}, {.val = 482}, {.val = 433}, {.val = 391}, {.val = 354}, {.val = 324}, {.val = 301}, {.val = 282}, {.val = 269}, {.val = 261}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 267}, {.val = 279}, {.val = 296}, {.val = 320}, {.val = 349}, {.val = 383}, {.val = 424}, {.val = 468}, {.val = 468}, {.val = 491}, {.val = 443}, {.val = 399}, {.val = 362}, {.val = 332}, {.val = 308}, {.val = 289}, {.val = 275}, {.val = 267}, {.val = 262}, {.val = 262}, {.val = 265}, {.val = 273}, {.val = 286}, {.val = 304}, {.val = 328}, {.val = 357}, {.val = 391}, {.val = 432}, {.val = 477}, {.val = 477}, {.val = 508}, {.val = 457}, {.val = 413}, {.val = 376}, {.val = 345}, {.val = 321}, {.val = 301}, {.val = 287}, {.val = 277}, {.val = 273}, {.val = 272}, {.val = 276}, {.val = 284}, {.val = 298}, {.val = 317}, {.val = 341}, {.val = 371}, {.val = 405}, {.val = 446}, {.val = 490}, {.val = 490}, {.val = 529}, {.val = 478}, {.val = 434}, {.val = 396}, {.val = 364}, {.val = 339}, {.val = 319}, {.val = 304}, {.val = 294}, {.val = 289}, {.val = 289}, {.val = 293}, {.val = 302}, {.val = 316}, {.val = 335}, {.val = 360}, {.val = 390}, {.val = 425}, {.val = 465}, {.val = 510}, {.val = 510}, {.val = 557}, {.val = 506}, {.val = 461}, {.val = 423}, {.val = 390}, {.val = 363}, {.val = 343}, {.val = 328}, {.val = 318}, {.val = 312}, {.val = 312}, {.val = 316}, {.val = 325}, {.val = 340}, {.val = 359}, {.val = 385}, {.val = 414}, {.val = 450}, {.val = 490}, {.val = 535}, {.val = 535}, {.val = 579}, {.val = 528}, {.val = 483}, {.val = 443}, {.val = 409}, {.val = 383}, {.val = 362}, {.val = 346}, {.val = 336}, {.val = 330}, {.val = 329}, {.val = 334}, {.val = 343}, {.val = 358}, {.val = 379}, {.val = 403}, {.val = 434}, {.val = 470}, {.val = 507}, {.val = 554}, {.val = 554}, {.val = 579}, {.val = 528}, {.val = 483}, {.val = 443}, {.val = 409}, {.val = 383}, {.val = 362}, {.val = 346}, {.val = 336}, {.val = 330}, {.val = 329}, {.val = 334}, {.val = 343}, {.val = 358}, {.val = 379}, {.val = 403}, {.val = 434}, {.val = 470}, {.val = 507}, {.val = 554}, {.val = 554}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_5210_config[] = {
    {.val = 512}, {.val = 469}, {.val = 432}, {.val = 402}, {.val = 376}, {.val = 354}, {.val = 338}, {.val = 327}, {.val = 326}, {.val = 321}, {.val = 314}, {.val = 317}, {.val = 324}, {.val = 336}, {.val = 351}, {.val = 370}, {.val = 394}, {.val = 422}, {.val = 454}, {.val = 490}, {.val = 490}, {.val = 489}, {.val = 447}, {.val = 409}, {.val = 379}, {.val = 353}, {.val = 332}, {.val = 317}, {.val = 306}, {.val = 302}, {.val = 299}, {.val = 292}, {.val = 295}, {.val = 302}, {.val = 313}, {.val = 328}, {.val = 347}, {.val = 372}, {.val = 400}, {.val = 432}, {.val = 468}, {.val = 468}, {.val = 472}, {.val = 429}, {.val = 392}, {.val = 362}, {.val = 335}, {.val = 315}, {.val = 299}, {.val = 288}, {.val = 282}, {.val = 278}, {.val = 276}, {.val = 280}, {.val = 285}, {.val = 296}, {.val = 310}, {.val = 330}, {.val = 355}, {.val = 383}, {.val = 415}, {.val = 452}, {.val = 452}, {.val = 460}, {.val = 417}, {.val = 380}, {.val = 349}, {.val = 324}, {.val = 303}, {.val = 287}, {.val = 276}, {.val = 269}, {.val = 266}, {.val = 265}, {.val = 269}, {.val = 274}, {.val = 285}, {.val = 298}, {.val = 318}, {.val = 342}, {.val = 371}, {.val = 404}, {.val = 440}, {.val = 440}, {.val = 453}, {.val = 410}, {.val = 373}, {.val = 342}, {.val = 316}, {.val = 295}, {.val = 280}, {.val = 269}, {.val = 262}, {.val = 259}, {.val = 259}, {.val = 261}, {.val = 267}, {.val = 277}, {.val = 291}, {.val = 311}, {.val = 335}, {.val = 363}, {.val = 396}, {.val = 433}, {.val = 433}, {.val = 451}, {.val = 408}, {.val = 370}, {.val = 339}, {.val = 313}, {.val = 293}, {.val = 277}, {.val = 266}, {.val = 259}, {.val = 256}, {.val = 255}, {.val = 258}, {.val = 264}, {.val = 274}, {.val = 288}, {.val = 307}, {.val = 332}, {.val = 360}, {.val = 393}, {.val = 429}, {.val = 429}, {.val = 452}, {.val = 409}, {.val = 373}, {.val = 341}, {.val = 315}, {.val = 294}, {.val = 278}, {.val = 267}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 265}, {.val = 275}, {.val = 290}, {.val = 309}, {.val = 333}, {.val = 363}, {.val = 395}, {.val = 432}, {.val = 432}, {.val = 460}, {.val = 417}, {.val = 380}, {.val = 348}, {.val = 321}, {.val = 300}, {.val = 283}, {.val = 272}, {.val = 265}, {.val = 261}, {.val = 260}, {.val = 264}, {.val = 270}, {.val = 280}, {.val = 296}, {.val = 316}, {.val = 341}, {.val = 369}, {.val = 402}, {.val = 439}, {.val = 439}, {.val = 474}, {.val = 428}, {.val = 391}, {.val = 360}, {.val = 332}, {.val = 311}, {.val = 294}, {.val = 282}, {.val = 274}, {.val = 270}, {.val = 270}, {.val = 273}, {.val = 279}, {.val = 292}, {.val = 307}, {.val = 328}, {.val = 352}, {.val = 380}, {.val = 414}, {.val = 450}, {.val = 450}, {.val = 494}, {.val = 448}, {.val = 408}, {.val = 376}, {.val = 349}, {.val = 326}, {.val = 310}, {.val = 297}, {.val = 289}, {.val = 285}, {.val = 284}, {.val = 288}, {.val = 296}, {.val = 307}, {.val = 323}, {.val = 344}, {.val = 368}, {.val = 397}, {.val = 430}, {.val = 467}, {.val = 467}, {.val = 517}, {.val = 473}, {.val = 433}, {.val = 398}, {.val = 370}, {.val = 348}, {.val = 329}, {.val = 317}, {.val = 309}, {.val = 305}, {.val = 304}, {.val = 308}, {.val = 316}, {.val = 328}, {.val = 345}, {.val = 365}, {.val = 389}, {.val = 419}, {.val = 451}, {.val = 489}, {.val = 489}, {.val = 536}, {.val = 491}, {.val = 452}, {.val = 415}, {.val = 387}, {.val = 364}, {.val = 346}, {.val = 332}, {.val = 324}, {.val = 320}, {.val = 320}, {.val = 323}, {.val = 332}, {.val = 344}, {.val = 361}, {.val = 381}, {.val = 405}, {.val = 434}, {.val = 467}, {.val = 504}, {.val = 504}, {.val = 536}, {.val = 491}, {.val = 452}, {.val = 415}, {.val = 387}, {.val = 364}, {.val = 346}, {.val = 332}, {.val = 324}, {.val = 320}, {.val = 320}, {.val = 323}, {.val = 332}, {.val = 344}, {.val = 361}, {.val = 381}, {.val = 405}, {.val = 434}, {.val = 467}, {.val = 504}, {.val = 504}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_5925_config[] = {
    {.val = 723}, {.val = 642}, {.val = 570}, {.val = 508}, {.val = 458}, {.val = 419}, {.val = 389}, {.val = 369}, {.val = 362}, {.val = 355}, {.val = 348}, {.val = 357}, {.val = 371}, {.val = 395}, {.val = 426}, {.val = 466}, {.val = 517}, {.val = 578}, {.val = 646}, {.val = 727}, {.val = 727}, {.val = 673}, {.val = 592}, {.val = 522}, {.val = 465}, {.val = 417}, {.val = 379}, {.val = 351}, {.val = 332}, {.val = 325}, {.val = 320}, {.val = 313}, {.val = 319}, {.val = 333}, {.val = 354}, {.val = 384}, {.val = 423}, {.val = 472}, {.val = 531}, {.val = 601}, {.val = 679}, {.val = 679}, {.val = 633}, {.val = 554}, {.val = 486}, {.val = 430}, {.val = 385}, {.val = 349}, {.val = 323}, {.val = 303}, {.val = 294}, {.val = 288}, {.val = 288}, {.val = 294}, {.val = 306}, {.val = 325}, {.val = 353}, {.val = 392}, {.val = 438}, {.val = 496}, {.val = 565}, {.val = 640}, {.val = 640}, {.val = 605}, {.val = 528}, {.val = 462}, {.val = 406}, {.val = 363}, {.val = 328}, {.val = 303}, {.val = 286}, {.val = 275}, {.val = 270}, {.val = 270}, {.val = 276}, {.val = 287}, {.val = 306}, {.val = 333}, {.val = 369}, {.val = 415}, {.val = 471}, {.val = 538}, {.val = 615}, {.val = 615}, {.val = 586}, {.val = 511}, {.val = 446}, {.val = 392}, {.val = 349}, {.val = 316}, {.val = 291}, {.val = 275}, {.val = 264}, {.val = 259}, {.val = 259}, {.val = 265}, {.val = 276}, {.val = 294}, {.val = 320}, {.val = 355}, {.val = 401}, {.val = 457}, {.val = 523}, {.val = 598}, {.val = 598}, {.val = 579}, {.val = 505}, {.val = 440}, {.val = 386}, {.val = 344}, {.val = 310}, {.val = 287}, {.val = 270}, {.val = 260}, {.val = 254}, {.val = 255}, {.val = 260}, {.val = 271}, {.val = 289}, {.val = 315}, {.val = 350}, {.val = 396}, {.val = 450}, {.val = 516}, {.val = 590}, {.val = 590}, {.val = 580}, {.val = 506}, {.val = 442}, {.val = 388}, {.val = 345}, {.val = 313}, {.val = 288}, {.val = 272}, {.val = 261}, {.val = 256}, {.val = 256}, {.val = 261}, {.val = 272}, {.val = 291}, {.val = 317}, {.val = 352}, {.val = 397}, {.val = 454}, {.val = 518}, {.val = 592}, {.val = 592}, {.val = 593}, {.val = 518}, {.val = 452}, {.val = 399}, {.val = 356}, {.val = 321}, {.val = 297}, {.val = 279}, {.val = 268}, {.val = 263}, {.val = 263}, {.val = 269}, {.val = 281}, {.val = 300}, {.val = 327}, {.val = 364}, {.val = 410}, {.val = 465}, {.val = 530}, {.val = 606}, {.val = 606}, {.val = 615}, {.val = 539}, {.val = 473}, {.val = 418}, {.val = 373}, {.val = 339}, {.val = 313}, {.val = 294}, {.val = 283}, {.val = 277}, {.val = 278}, {.val = 284}, {.val = 296}, {.val = 317}, {.val = 345}, {.val = 383}, {.val = 429}, {.val = 487}, {.val = 553}, {.val = 629}, {.val = 629}, {.val = 649}, {.val = 572}, {.val = 504}, {.val = 447}, {.val = 401}, {.val = 364}, {.val = 337}, {.val = 318}, {.val = 306}, {.val = 300}, {.val = 300}, {.val = 308}, {.val = 321}, {.val = 342}, {.val = 371}, {.val = 411}, {.val = 459}, {.val = 518}, {.val = 585}, {.val = 663}, {.val = 663}, {.val = 690}, {.val = 612}, {.val = 546}, {.val = 486}, {.val = 439}, {.val = 401}, {.val = 371}, {.val = 351}, {.val = 338}, {.val = 331}, {.val = 332}, {.val = 340}, {.val = 354}, {.val = 377}, {.val = 408}, {.val = 448}, {.val = 498}, {.val = 558}, {.val = 629}, {.val = 705}, {.val = 705}, {.val = 724}, {.val = 645}, {.val = 575}, {.val = 516}, {.val = 467}, {.val = 427}, {.val = 398}, {.val = 378}, {.val = 364}, {.val = 356}, {.val = 358}, {.val = 365}, {.val = 380}, {.val = 404}, {.val = 437}, {.val = 480}, {.val = 530}, {.val = 589}, {.val = 660}, {.val = 738}, {.val = 738}, {.val = 724}, {.val = 645}, {.val = 575}, {.val = 516}, {.val = 467}, {.val = 427}, {.val = 398}, {.val = 378}, {.val = 364}, {.val = 356}, {.val = 358}, {.val = 365}, {.val = 380}, {.val = 404}, {.val = 437}, {.val = 480}, {.val = 530}, {.val = 589}, {.val = 660}, {.val = 738}, {.val = 738}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_5925_config[] = {
    {.val = 562}, {.val = 513}, {.val = 469}, {.val = 431}, {.val = 399}, {.val = 374}, {.val = 355}, {.val = 341}, {.val = 337}, {.val = 333}, {.val = 325}, {.val = 329}, {.val = 338}, {.val = 354}, {.val = 372}, {.val = 396}, {.val = 425}, {.val = 460}, {.val = 501}, {.val = 545}, {.val = 545}, {.val = 532}, {.val = 483}, {.val = 439}, {.val = 403}, {.val = 372}, {.val = 347}, {.val = 328}, {.val = 316}, {.val = 311}, {.val = 307}, {.val = 300}, {.val = 304}, {.val = 312}, {.val = 325}, {.val = 344}, {.val = 368}, {.val = 397}, {.val = 432}, {.val = 473}, {.val = 517}, {.val = 517}, {.val = 507}, {.val = 458}, {.val = 415}, {.val = 379}, {.val = 350}, {.val = 326}, {.val = 307}, {.val = 294}, {.val = 287}, {.val = 283}, {.val = 281}, {.val = 285}, {.val = 293}, {.val = 305}, {.val = 323}, {.val = 347}, {.val = 376}, {.val = 411}, {.val = 450}, {.val = 495}, {.val = 495}, {.val = 489}, {.val = 440}, {.val = 398}, {.val = 363}, {.val = 334}, {.val = 311}, {.val = 293}, {.val = 280}, {.val = 273}, {.val = 269}, {.val = 268}, {.val = 271}, {.val = 279}, {.val = 291}, {.val = 309}, {.val = 331}, {.val = 361}, {.val = 394}, {.val = 435}, {.val = 480}, {.val = 480}, {.val = 478}, {.val = 429}, {.val = 388}, {.val = 353}, {.val = 324}, {.val = 301}, {.val = 284}, {.val = 271}, {.val = 264}, {.val = 260}, {.val = 260}, {.val = 263}, {.val = 270}, {.val = 282}, {.val = 299}, {.val = 322}, {.val = 350}, {.val = 385}, {.val = 424}, {.val = 470}, {.val = 470}, {.val = 473}, {.val = 424}, {.val = 382}, {.val = 348}, {.val = 319}, {.val = 296}, {.val = 279}, {.val = 268}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 266}, {.val = 278}, {.val = 295}, {.val = 318}, {.val = 347}, {.val = 380}, {.val = 420}, {.val = 465}, {.val = 465}, {.val = 472}, {.val = 425}, {.val = 383}, {.val = 349}, {.val = 320}, {.val = 297}, {.val = 280}, {.val = 268}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 267}, {.val = 279}, {.val = 296}, {.val = 318}, {.val = 348}, {.val = 382}, {.val = 421}, {.val = 467}, {.val = 467}, {.val = 479}, {.val = 431}, {.val = 390}, {.val = 355}, {.val = 326}, {.val = 303}, {.val = 286}, {.val = 273}, {.val = 265}, {.val = 261}, {.val = 261}, {.val = 265}, {.val = 273}, {.val = 285}, {.val = 302}, {.val = 326}, {.val = 354}, {.val = 389}, {.val = 429}, {.val = 474}, {.val = 474}, {.val = 491}, {.val = 444}, {.val = 403}, {.val = 367}, {.val = 338}, {.val = 315}, {.val = 296}, {.val = 283}, {.val = 275}, {.val = 271}, {.val = 271}, {.val = 274}, {.val = 283}, {.val = 296}, {.val = 314}, {.val = 338}, {.val = 368}, {.val = 402}, {.val = 441}, {.val = 487}, {.val = 487}, {.val = 511}, {.val = 463}, {.val = 421}, {.val = 385}, {.val = 354}, {.val = 331}, {.val = 313}, {.val = 299}, {.val = 290}, {.val = 286}, {.val = 286}, {.val = 291}, {.val = 300}, {.val = 313}, {.val = 332}, {.val = 356}, {.val = 385}, {.val = 421}, {.val = 461}, {.val = 506}, {.val = 506}, {.val = 534}, {.val = 486}, {.val = 444}, {.val = 408}, {.val = 378}, {.val = 354}, {.val = 335}, {.val = 321}, {.val = 312}, {.val = 307}, {.val = 307}, {.val = 312}, {.val = 322}, {.val = 336}, {.val = 355}, {.val = 379}, {.val = 409}, {.val = 445}, {.val = 484}, {.val = 529}, {.val = 529}, {.val = 553}, {.val = 506}, {.val = 462}, {.val = 426}, {.val = 395}, {.val = 371}, {.val = 352}, {.val = 338}, {.val = 329}, {.val = 324}, {.val = 324}, {.val = 329}, {.val = 338}, {.val = 354}, {.val = 373}, {.val = 397}, {.val = 427}, {.val = 461}, {.val = 504}, {.val = 547}, {.val = 547}, {.val = 553}, {.val = 506}, {.val = 462}, {.val = 426}, {.val = 395}, {.val = 371}, {.val = 352}, {.val = 338}, {.val = 329}, {.val = 324}, {.val = 324}, {.val = 329}, {.val = 338}, {.val = 354}, {.val = 373}, {.val = 397}, {.val = 427}, {.val = 461}, {.val = 504}, {.val = 547}, {.val = 547}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_5925_config[] = {
    {.val = 547}, {.val = 500}, {.val = 458}, {.val = 423}, {.val = 392}, {.val = 367}, {.val = 348}, {.val = 335}, {.val = 333}, {.val = 327}, {.val = 320}, {.val = 325}, {.val = 333}, {.val = 349}, {.val = 367}, {.val = 389}, {.val = 419}, {.val = 453}, {.val = 492}, {.val = 535}, {.val = 535}, {.val = 522}, {.val = 474}, {.val = 432}, {.val = 396}, {.val = 367}, {.val = 343}, {.val = 324}, {.val = 312}, {.val = 307}, {.val = 303}, {.val = 296}, {.val = 300}, {.val = 309}, {.val = 322}, {.val = 341}, {.val = 364}, {.val = 393}, {.val = 427}, {.val = 465}, {.val = 509}, {.val = 509}, {.val = 501}, {.val = 453}, {.val = 412}, {.val = 376}, {.val = 347}, {.val = 323}, {.val = 305}, {.val = 291}, {.val = 285}, {.val = 280}, {.val = 279}, {.val = 282}, {.val = 290}, {.val = 303}, {.val = 321}, {.val = 344}, {.val = 372}, {.val = 406}, {.val = 445}, {.val = 489}, {.val = 489}, {.val = 486}, {.val = 439}, {.val = 397}, {.val = 362}, {.val = 333}, {.val = 309}, {.val = 292}, {.val = 279}, {.val = 271}, {.val = 267}, {.val = 267}, {.val = 269}, {.val = 277}, {.val = 289}, {.val = 306}, {.val = 329}, {.val = 358}, {.val = 391}, {.val = 431}, {.val = 475}, {.val = 475}, {.val = 479}, {.val = 431}, {.val = 389}, {.val = 353}, {.val = 325}, {.val = 301}, {.val = 284}, {.val = 271}, {.val = 263}, {.val = 260}, {.val = 259}, {.val = 262}, {.val = 269}, {.val = 281}, {.val = 298}, {.val = 321}, {.val = 349}, {.val = 382}, {.val = 421}, {.val = 466}, {.val = 466}, {.val = 477}, {.val = 428}, {.val = 386}, {.val = 351}, {.val = 321}, {.val = 298}, {.val = 280}, {.val = 268}, {.val = 260}, {.val = 256}, {.val = 255}, {.val = 258}, {.val = 266}, {.val = 277}, {.val = 294}, {.val = 317}, {.val = 345}, {.val = 379}, {.val = 418}, {.val = 462}, {.val = 462}, {.val = 479}, {.val = 431}, {.val = 388}, {.val = 353}, {.val = 323}, {.val = 300}, {.val = 282}, {.val = 269}, {.val = 261}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 266}, {.val = 279}, {.val = 296}, {.val = 319}, {.val = 347}, {.val = 381}, {.val = 420}, {.val = 463}, {.val = 463}, {.val = 488}, {.val = 440}, {.val = 396}, {.val = 361}, {.val = 330}, {.val = 307}, {.val = 288}, {.val = 275}, {.val = 266}, {.val = 262}, {.val = 262}, {.val = 265}, {.val = 273}, {.val = 285}, {.val = 304}, {.val = 327}, {.val = 355}, {.val = 389}, {.val = 428}, {.val = 472}, {.val = 472}, {.val = 503}, {.val = 454}, {.val = 412}, {.val = 375}, {.val = 344}, {.val = 320}, {.val = 300}, {.val = 287}, {.val = 277}, {.val = 273}, {.val = 272}, {.val = 275}, {.val = 284}, {.val = 297}, {.val = 316}, {.val = 340}, {.val = 369}, {.val = 402}, {.val = 442}, {.val = 486}, {.val = 486}, {.val = 525}, {.val = 475}, {.val = 432}, {.val = 395}, {.val = 364}, {.val = 338}, {.val = 318}, {.val = 303}, {.val = 294}, {.val = 289}, {.val = 288}, {.val = 292}, {.val = 301}, {.val = 315}, {.val = 334}, {.val = 358}, {.val = 387}, {.val = 422}, {.val = 461}, {.val = 505}, {.val = 505}, {.val = 553}, {.val = 503}, {.val = 458}, {.val = 421}, {.val = 389}, {.val = 363}, {.val = 342}, {.val = 327}, {.val = 317}, {.val = 311}, {.val = 311}, {.val = 315}, {.val = 324}, {.val = 339}, {.val = 358}, {.val = 382}, {.val = 412}, {.val = 446}, {.val = 486}, {.val = 530}, {.val = 530}, {.val = 573}, {.val = 525}, {.val = 479}, {.val = 441}, {.val = 407}, {.val = 381}, {.val = 361}, {.val = 345}, {.val = 335}, {.val = 329}, {.val = 328}, {.val = 333}, {.val = 343}, {.val = 357}, {.val = 376}, {.val = 402}, {.val = 432}, {.val = 467}, {.val = 505}, {.val = 547}, {.val = 547}, {.val = 573}, {.val = 525}, {.val = 479}, {.val = 441}, {.val = 407}, {.val = 381}, {.val = 361}, {.val = 345}, {.val = 335}, {.val = 329}, {.val = 328}, {.val = 333}, {.val = 343}, {.val = 357}, {.val = 376}, {.val = 402}, {.val = 432}, {.val = 467}, {.val = 505}, {.val = 547}, {.val = 547}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_5925_config[] = {
    {.val = 508}, {.val = 467}, {.val = 431}, {.val = 400}, {.val = 374}, {.val = 354}, {.val = 338}, {.val = 327}, {.val = 326}, {.val = 320}, {.val = 313}, {.val = 315}, {.val = 323}, {.val = 335}, {.val = 350}, {.val = 368}, {.val = 393}, {.val = 421}, {.val = 452}, {.val = 489}, {.val = 489}, {.val = 487}, {.val = 444}, {.val = 409}, {.val = 378}, {.val = 352}, {.val = 332}, {.val = 316}, {.val = 306}, {.val = 302}, {.val = 299}, {.val = 292}, {.val = 295}, {.val = 301}, {.val = 312}, {.val = 327}, {.val = 346}, {.val = 371}, {.val = 398}, {.val = 430}, {.val = 467}, {.val = 467}, {.val = 470}, {.val = 427}, {.val = 391}, {.val = 361}, {.val = 336}, {.val = 314}, {.val = 299}, {.val = 287}, {.val = 282}, {.val = 278}, {.val = 276}, {.val = 279}, {.val = 285}, {.val = 296}, {.val = 310}, {.val = 329}, {.val = 352}, {.val = 381}, {.val = 413}, {.val = 449}, {.val = 449}, {.val = 457}, {.val = 415}, {.val = 380}, {.val = 348}, {.val = 323}, {.val = 302}, {.val = 287}, {.val = 276}, {.val = 269}, {.val = 266}, {.val = 266}, {.val = 268}, {.val = 274}, {.val = 284}, {.val = 298}, {.val = 317}, {.val = 341}, {.val = 369}, {.val = 402}, {.val = 438}, {.val = 438}, {.val = 452}, {.val = 409}, {.val = 372}, {.val = 341}, {.val = 315}, {.val = 296}, {.val = 280}, {.val = 269}, {.val = 262}, {.val = 259}, {.val = 259}, {.val = 261}, {.val = 267}, {.val = 276}, {.val = 291}, {.val = 310}, {.val = 333}, {.val = 362}, {.val = 394}, {.val = 430}, {.val = 430}, {.val = 449}, {.val = 406}, {.val = 370}, {.val = 338}, {.val = 313}, {.val = 292}, {.val = 277}, {.val = 266}, {.val = 259}, {.val = 256}, {.val = 256}, {.val = 258}, {.val = 264}, {.val = 274}, {.val = 288}, {.val = 307}, {.val = 330}, {.val = 359}, {.val = 391}, {.val = 427}, {.val = 427}, {.val = 451}, {.val = 409}, {.val = 372}, {.val = 340}, {.val = 314}, {.val = 294}, {.val = 277}, {.val = 267}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 258}, {.val = 264}, {.val = 274}, {.val = 289}, {.val = 308}, {.val = 332}, {.val = 361}, {.val = 393}, {.val = 430}, {.val = 430}, {.val = 459}, {.val = 416}, {.val = 379}, {.val = 347}, {.val = 321}, {.val = 300}, {.val = 283}, {.val = 272}, {.val = 264}, {.val = 261}, {.val = 260}, {.val = 264}, {.val = 269}, {.val = 280}, {.val = 296}, {.val = 315}, {.val = 339}, {.val = 368}, {.val = 400}, {.val = 437}, {.val = 437}, {.val = 472}, {.val = 428}, {.val = 391}, {.val = 359}, {.val = 332}, {.val = 311}, {.val = 294}, {.val = 282}, {.val = 274}, {.val = 270}, {.val = 270}, {.val = 273}, {.val = 280}, {.val = 291}, {.val = 306}, {.val = 327}, {.val = 351}, {.val = 379}, {.val = 412}, {.val = 448}, {.val = 448}, {.val = 493}, {.val = 447}, {.val = 408}, {.val = 375}, {.val = 348}, {.val = 326}, {.val = 309}, {.val = 297}, {.val = 289}, {.val = 284}, {.val = 283}, {.val = 287}, {.val = 295}, {.val = 306}, {.val = 322}, {.val = 343}, {.val = 366}, {.val = 395}, {.val = 427}, {.val = 464}, {.val = 464}, {.val = 517}, {.val = 471}, {.val = 431}, {.val = 398}, {.val = 369}, {.val = 347}, {.val = 329}, {.val = 317}, {.val = 308}, {.val = 304}, {.val = 303}, {.val = 308}, {.val = 316}, {.val = 327}, {.val = 343}, {.val = 364}, {.val = 388}, {.val = 417}, {.val = 449}, {.val = 485}, {.val = 485}, {.val = 535}, {.val = 490}, {.val = 449}, {.val = 415}, {.val = 386}, {.val = 363}, {.val = 345}, {.val = 332}, {.val = 324}, {.val = 319}, {.val = 318}, {.val = 324}, {.val = 332}, {.val = 344}, {.val = 359}, {.val = 379}, {.val = 405}, {.val = 433}, {.val = 465}, {.val = 501}, {.val = 501}, {.val = 535}, {.val = 490}, {.val = 449}, {.val = 415}, {.val = 386}, {.val = 363}, {.val = 345}, {.val = 332}, {.val = 324}, {.val = 319}, {.val = 318}, {.val = 324}, {.val = 332}, {.val = 344}, {.val = 359}, {.val = 379}, {.val = 405}, {.val = 433}, {.val = 465}, {.val = 501}, {.val = 501}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_6254_config[] = {
    {.val = 716}, {.val = 635}, {.val = 564}, {.val = 506}, {.val = 457}, {.val = 417}, {.val = 387}, {.val = 368}, {.val = 361}, {.val = 354}, {.val = 347}, {.val = 356}, {.val = 370}, {.val = 394}, {.val = 424}, {.val = 463}, {.val = 514}, {.val = 573}, {.val = 642}, {.val = 717}, {.val = 717}, {.val = 666}, {.val = 588}, {.val = 520}, {.val = 462}, {.val = 415}, {.val = 378}, {.val = 350}, {.val = 332}, {.val = 325}, {.val = 319}, {.val = 313}, {.val = 319}, {.val = 332}, {.val = 353}, {.val = 383}, {.val = 420}, {.val = 470}, {.val = 527}, {.val = 595}, {.val = 669}, {.val = 669}, {.val = 627}, {.val = 549}, {.val = 484}, {.val = 428}, {.val = 384}, {.val = 348}, {.val = 323}, {.val = 303}, {.val = 295}, {.val = 288}, {.val = 288}, {.val = 294}, {.val = 306}, {.val = 325}, {.val = 352}, {.val = 390}, {.val = 435}, {.val = 493}, {.val = 559}, {.val = 635}, {.val = 635}, {.val = 598}, {.val = 524}, {.val = 459}, {.val = 405}, {.val = 361}, {.val = 328}, {.val = 303}, {.val = 286}, {.val = 275}, {.val = 270}, {.val = 270}, {.val = 276}, {.val = 287}, {.val = 306}, {.val = 333}, {.val = 368}, {.val = 413}, {.val = 469}, {.val = 533}, {.val = 608}, {.val = 608}, {.val = 582}, {.val = 506}, {.val = 444}, {.val = 391}, {.val = 349}, {.val = 316}, {.val = 292}, {.val = 275}, {.val = 264}, {.val = 260}, {.val = 260}, {.val = 265}, {.val = 276}, {.val = 294}, {.val = 319}, {.val = 355}, {.val = 399}, {.val = 454}, {.val = 519}, {.val = 592}, {.val = 592}, {.val = 574}, {.val = 500}, {.val = 437}, {.val = 385}, {.val = 344}, {.val = 311}, {.val = 287}, {.val = 270}, {.val = 260}, {.val = 255}, {.val = 255}, {.val = 260}, {.val = 271}, {.val = 289}, {.val = 315}, {.val = 349}, {.val = 394}, {.val = 448}, {.val = 511}, {.val = 586}, {.val = 586}, {.val = 576}, {.val = 503}, {.val = 440}, {.val = 388}, {.val = 346}, {.val = 313}, {.val = 288}, {.val = 272}, {.val = 261}, {.val = 256}, {.val = 256}, {.val = 261}, {.val = 273}, {.val = 291}, {.val = 316}, {.val = 351}, {.val = 395}, {.val = 450}, {.val = 515}, {.val = 586}, {.val = 586}, {.val = 587}, {.val = 515}, {.val = 450}, {.val = 398}, {.val = 354}, {.val = 321}, {.val = 297}, {.val = 279}, {.val = 269}, {.val = 263}, {.val = 263}, {.val = 269}, {.val = 281}, {.val = 300}, {.val = 327}, {.val = 362}, {.val = 407}, {.val = 462}, {.val = 527}, {.val = 600}, {.val = 600}, {.val = 610}, {.val = 535}, {.val = 469}, {.val = 417}, {.val = 372}, {.val = 338}, {.val = 313}, {.val = 294}, {.val = 283}, {.val = 277}, {.val = 278}, {.val = 284}, {.val = 297}, {.val = 316}, {.val = 344}, {.val = 381}, {.val = 428}, {.val = 483}, {.val = 546}, {.val = 621}, {.val = 621}, {.val = 643}, {.val = 567}, {.val = 501}, {.val = 445}, {.val = 400}, {.val = 364}, {.val = 337}, {.val = 318}, {.val = 305}, {.val = 300}, {.val = 300}, {.val = 307}, {.val = 321}, {.val = 342}, {.val = 371}, {.val = 408}, {.val = 456}, {.val = 513}, {.val = 579}, {.val = 655}, {.val = 655}, {.val = 685}, {.val = 607}, {.val = 540}, {.val = 483}, {.val = 436}, {.val = 399}, {.val = 370}, {.val = 350}, {.val = 337}, {.val = 330}, {.val = 332}, {.val = 339}, {.val = 354}, {.val = 375}, {.val = 406}, {.val = 446}, {.val = 495}, {.val = 554}, {.val = 622}, {.val = 696}, {.val = 696}, {.val = 716}, {.val = 637}, {.val = 571}, {.val = 514}, {.val = 465}, {.val = 425}, {.val = 396}, {.val = 375}, {.val = 362}, {.val = 356}, {.val = 356}, {.val = 365}, {.val = 380}, {.val = 403}, {.val = 433}, {.val = 475}, {.val = 524}, {.val = 585}, {.val = 654}, {.val = 726}, {.val = 726}, {.val = 716}, {.val = 637}, {.val = 571}, {.val = 514}, {.val = 465}, {.val = 425}, {.val = 396}, {.val = 375}, {.val = 362}, {.val = 356}, {.val = 356}, {.val = 365}, {.val = 380}, {.val = 403}, {.val = 433}, {.val = 475}, {.val = 524}, {.val = 585}, {.val = 654}, {.val = 726}, {.val = 726}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_6254_config[] = {
    {.val = 561}, {.val = 511}, {.val = 467}, {.val = 430}, {.val = 399}, {.val = 373}, {.val = 354}, {.val = 340}, {.val = 337}, {.val = 332}, {.val = 324}, {.val = 329}, {.val = 337}, {.val = 352}, {.val = 371}, {.val = 394}, {.val = 424}, {.val = 458}, {.val = 498}, {.val = 543}, {.val = 543}, {.val = 529}, {.val = 480}, {.val = 437}, {.val = 402}, {.val = 370}, {.val = 347}, {.val = 328}, {.val = 315}, {.val = 311}, {.val = 307}, {.val = 300}, {.val = 304}, {.val = 312}, {.val = 325}, {.val = 344}, {.val = 367}, {.val = 396}, {.val = 431}, {.val = 471}, {.val = 515}, {.val = 515}, {.val = 505}, {.val = 456}, {.val = 414}, {.val = 378}, {.val = 349}, {.val = 325}, {.val = 307}, {.val = 293}, {.val = 287}, {.val = 282}, {.val = 281}, {.val = 285}, {.val = 292}, {.val = 305}, {.val = 323}, {.val = 346}, {.val = 374}, {.val = 410}, {.val = 448}, {.val = 494}, {.val = 494}, {.val = 488}, {.val = 439}, {.val = 397}, {.val = 362}, {.val = 333}, {.val = 311}, {.val = 292}, {.val = 280}, {.val = 272}, {.val = 268}, {.val = 268}, {.val = 271}, {.val = 278}, {.val = 291}, {.val = 308}, {.val = 331}, {.val = 359}, {.val = 393}, {.val = 433}, {.val = 479}, {.val = 479}, {.val = 476}, {.val = 428}, {.val = 387}, {.val = 352}, {.val = 323}, {.val = 301}, {.val = 283}, {.val = 271}, {.val = 264}, {.val = 260}, {.val = 260}, {.val = 263}, {.val = 270}, {.val = 281}, {.val = 299}, {.val = 321}, {.val = 350}, {.val = 384}, {.val = 423}, {.val = 469}, {.val = 469}, {.val = 471}, {.val = 423}, {.val = 382}, {.val = 347}, {.val = 319}, {.val = 296}, {.val = 279}, {.val = 267}, {.val = 259}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 266}, {.val = 278}, {.val = 294}, {.val = 317}, {.val = 346}, {.val = 379}, {.val = 419}, {.val = 463}, {.val = 463}, {.val = 471}, {.val = 424}, {.val = 382}, {.val = 348}, {.val = 319}, {.val = 297}, {.val = 280}, {.val = 268}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 267}, {.val = 278}, {.val = 296}, {.val = 318}, {.val = 346}, {.val = 381}, {.val = 420}, {.val = 465}, {.val = 465}, {.val = 478}, {.val = 430}, {.val = 389}, {.val = 354}, {.val = 326}, {.val = 303}, {.val = 285}, {.val = 272}, {.val = 265}, {.val = 261}, {.val = 261}, {.val = 264}, {.val = 272}, {.val = 285}, {.val = 302}, {.val = 325}, {.val = 354}, {.val = 388}, {.val = 428}, {.val = 472}, {.val = 472}, {.val = 490}, {.val = 443}, {.val = 401}, {.val = 367}, {.val = 337}, {.val = 314}, {.val = 296}, {.val = 283}, {.val = 275}, {.val = 271}, {.val = 271}, {.val = 274}, {.val = 283}, {.val = 296}, {.val = 314}, {.val = 337}, {.val = 366}, {.val = 401}, {.val = 440}, {.val = 486}, {.val = 486}, {.val = 508}, {.val = 461}, {.val = 419}, {.val = 383}, {.val = 354}, {.val = 330}, {.val = 312}, {.val = 299}, {.val = 289}, {.val = 286}, {.val = 286}, {.val = 290}, {.val = 299}, {.val = 313}, {.val = 331}, {.val = 355}, {.val = 384}, {.val = 419}, {.val = 458}, {.val = 504}, {.val = 504}, {.val = 532}, {.val = 485}, {.val = 443}, {.val = 407}, {.val = 377}, {.val = 353}, {.val = 334}, {.val = 320}, {.val = 311}, {.val = 307}, {.val = 307}, {.val = 312}, {.val = 321}, {.val = 335}, {.val = 354}, {.val = 378}, {.val = 408}, {.val = 443}, {.val = 483}, {.val = 527}, {.val = 527}, {.val = 551}, {.val = 503}, {.val = 460}, {.val = 425}, {.val = 394}, {.val = 370}, {.val = 351}, {.val = 337}, {.val = 328}, {.val = 323}, {.val = 323}, {.val = 328}, {.val = 337}, {.val = 352}, {.val = 371}, {.val = 395}, {.val = 426}, {.val = 460}, {.val = 501}, {.val = 547}, {.val = 547}, {.val = 551}, {.val = 503}, {.val = 460}, {.val = 425}, {.val = 394}, {.val = 370}, {.val = 351}, {.val = 337}, {.val = 328}, {.val = 323}, {.val = 323}, {.val = 328}, {.val = 337}, {.val = 352}, {.val = 371}, {.val = 395}, {.val = 426}, {.val = 460}, {.val = 501}, {.val = 547}, {.val = 547}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_6254_config[] = {
    {.val = 545}, {.val = 498}, {.val = 457}, {.val = 421}, {.val = 391}, {.val = 366}, {.val = 348}, {.val = 335}, {.val = 332}, {.val = 326}, {.val = 319}, {.val = 324}, {.val = 333}, {.val = 348}, {.val = 366}, {.val = 389}, {.val = 418}, {.val = 452}, {.val = 490}, {.val = 534}, {.val = 534}, {.val = 520}, {.val = 472}, {.val = 431}, {.val = 395}, {.val = 366}, {.val = 342}, {.val = 324}, {.val = 312}, {.val = 307}, {.val = 303}, {.val = 296}, {.val = 300}, {.val = 308}, {.val = 322}, {.val = 340}, {.val = 363}, {.val = 392}, {.val = 425}, {.val = 464}, {.val = 507}, {.val = 507}, {.val = 499}, {.val = 451}, {.val = 411}, {.val = 376}, {.val = 346}, {.val = 323}, {.val = 305}, {.val = 291}, {.val = 284}, {.val = 280}, {.val = 279}, {.val = 282}, {.val = 290}, {.val = 302}, {.val = 320}, {.val = 343}, {.val = 371}, {.val = 405}, {.val = 443}, {.val = 487}, {.val = 487}, {.val = 485}, {.val = 438}, {.val = 396}, {.val = 362}, {.val = 332}, {.val = 309}, {.val = 291}, {.val = 279}, {.val = 271}, {.val = 267}, {.val = 266}, {.val = 269}, {.val = 277}, {.val = 289}, {.val = 306}, {.val = 329}, {.val = 357}, {.val = 390}, {.val = 429}, {.val = 473}, {.val = 473}, {.val = 477}, {.val = 430}, {.val = 388}, {.val = 353}, {.val = 324}, {.val = 301}, {.val = 283}, {.val = 271}, {.val = 263}, {.val = 259}, {.val = 259}, {.val = 262}, {.val = 268}, {.val = 281}, {.val = 298}, {.val = 320}, {.val = 348}, {.val = 381}, {.val = 420}, {.val = 464}, {.val = 464}, {.val = 475}, {.val = 426}, {.val = 385}, {.val = 350}, {.val = 321}, {.val = 297}, {.val = 280}, {.val = 267}, {.val = 260}, {.val = 255}, {.val = 255}, {.val = 258}, {.val = 265}, {.val = 277}, {.val = 294}, {.val = 317}, {.val = 345}, {.val = 378}, {.val = 416}, {.val = 460}, {.val = 460}, {.val = 477}, {.val = 429}, {.val = 388}, {.val = 352}, {.val = 323}, {.val = 300}, {.val = 281}, {.val = 269}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 266}, {.val = 279}, {.val = 295}, {.val = 318}, {.val = 347}, {.val = 380}, {.val = 418}, {.val = 462}, {.val = 462}, {.val = 486}, {.val = 438}, {.val = 396}, {.val = 360}, {.val = 331}, {.val = 307}, {.val = 288}, {.val = 275}, {.val = 266}, {.val = 262}, {.val = 261}, {.val = 265}, {.val = 272}, {.val = 285}, {.val = 303}, {.val = 326}, {.val = 354}, {.val = 388}, {.val = 427}, {.val = 470}, {.val = 470}, {.val = 501}, {.val = 453}, {.val = 411}, {.val = 374}, {.val = 344}, {.val = 319}, {.val = 300}, {.val = 286}, {.val = 277}, {.val = 272}, {.val = 272}, {.val = 275}, {.val = 284}, {.val = 297}, {.val = 315}, {.val = 339}, {.val = 368}, {.val = 401}, {.val = 440}, {.val = 483}, {.val = 483}, {.val = 523}, {.val = 473}, {.val = 431}, {.val = 393}, {.val = 363}, {.val = 337}, {.val = 318}, {.val = 303}, {.val = 294}, {.val = 289}, {.val = 288}, {.val = 292}, {.val = 301}, {.val = 315}, {.val = 334}, {.val = 357}, {.val = 387}, {.val = 420}, {.val = 459}, {.val = 504}, {.val = 504}, {.val = 551}, {.val = 501}, {.val = 457}, {.val = 420}, {.val = 388}, {.val = 362}, {.val = 342}, {.val = 326}, {.val = 317}, {.val = 311}, {.val = 311}, {.val = 315}, {.val = 324}, {.val = 338}, {.val = 357}, {.val = 382}, {.val = 411}, {.val = 446}, {.val = 485}, {.val = 527}, {.val = 527}, {.val = 570}, {.val = 522}, {.val = 477}, {.val = 438}, {.val = 406}, {.val = 381}, {.val = 359}, {.val = 345}, {.val = 335}, {.val = 328}, {.val = 328}, {.val = 332}, {.val = 342}, {.val = 357}, {.val = 377}, {.val = 400}, {.val = 430}, {.val = 464}, {.val = 501}, {.val = 546}, {.val = 546}, {.val = 570}, {.val = 522}, {.val = 477}, {.val = 438}, {.val = 406}, {.val = 381}, {.val = 359}, {.val = 345}, {.val = 335}, {.val = 328}, {.val = 328}, {.val = 332}, {.val = 342}, {.val = 357}, {.val = 377}, {.val = 400}, {.val = 430}, {.val = 464}, {.val = 501}, {.val = 546}, {.val = 546}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_6254_config[] = {
    {.val = 508}, {.val = 467}, {.val = 431}, {.val = 401}, {.val = 374}, {.val = 354}, {.val = 338}, {.val = 327}, {.val = 325}, {.val = 320}, {.val = 313}, {.val = 316}, {.val = 323}, {.val = 335}, {.val = 350}, {.val = 367}, {.val = 391}, {.val = 419}, {.val = 451}, {.val = 487}, {.val = 487}, {.val = 487}, {.val = 444}, {.val = 408}, {.val = 378}, {.val = 352}, {.val = 332}, {.val = 316}, {.val = 306}, {.val = 302}, {.val = 299}, {.val = 292}, {.val = 295}, {.val = 301}, {.val = 312}, {.val = 327}, {.val = 346}, {.val = 370}, {.val = 397}, {.val = 429}, {.val = 464}, {.val = 464}, {.val = 471}, {.val = 427}, {.val = 391}, {.val = 360}, {.val = 335}, {.val = 315}, {.val = 299}, {.val = 287}, {.val = 282}, {.val = 278}, {.val = 276}, {.val = 279}, {.val = 285}, {.val = 296}, {.val = 310}, {.val = 330}, {.val = 353}, {.val = 381}, {.val = 413}, {.val = 449}, {.val = 449}, {.val = 457}, {.val = 416}, {.val = 380}, {.val = 349}, {.val = 323}, {.val = 303}, {.val = 287}, {.val = 276}, {.val = 269}, {.val = 266}, {.val = 266}, {.val = 268}, {.val = 274}, {.val = 284}, {.val = 298}, {.val = 317}, {.val = 341}, {.val = 369}, {.val = 401}, {.val = 437}, {.val = 437}, {.val = 451}, {.val = 409}, {.val = 373}, {.val = 342}, {.val = 316}, {.val = 295}, {.val = 280}, {.val = 269}, {.val = 263}, {.val = 260}, {.val = 259}, {.val = 261}, {.val = 267}, {.val = 277}, {.val = 291}, {.val = 310}, {.val = 333}, {.val = 362}, {.val = 394}, {.val = 430}, {.val = 430}, {.val = 449}, {.val = 406}, {.val = 370}, {.val = 338}, {.val = 313}, {.val = 292}, {.val = 277}, {.val = 266}, {.val = 259}, {.val = 256}, {.val = 256}, {.val = 258}, {.val = 264}, {.val = 273}, {.val = 287}, {.val = 307}, {.val = 330}, {.val = 359}, {.val = 391}, {.val = 428}, {.val = 428}, {.val = 452}, {.val = 409}, {.val = 372}, {.val = 341}, {.val = 315}, {.val = 294}, {.val = 278}, {.val = 267}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 258}, {.val = 264}, {.val = 275}, {.val = 289}, {.val = 308}, {.val = 332}, {.val = 361}, {.val = 393}, {.val = 429}, {.val = 429}, {.val = 460}, {.val = 416}, {.val = 379}, {.val = 348}, {.val = 321}, {.val = 300}, {.val = 284}, {.val = 272}, {.val = 265}, {.val = 261}, {.val = 260}, {.val = 263}, {.val = 270}, {.val = 281}, {.val = 296}, {.val = 315}, {.val = 339}, {.val = 367}, {.val = 400}, {.val = 436}, {.val = 436}, {.val = 472}, {.val = 429}, {.val = 390}, {.val = 358}, {.val = 332}, {.val = 311}, {.val = 294}, {.val = 282}, {.val = 274}, {.val = 270}, {.val = 270}, {.val = 273}, {.val = 280}, {.val = 291}, {.val = 306}, {.val = 326}, {.val = 351}, {.val = 379}, {.val = 411}, {.val = 448}, {.val = 448}, {.val = 491}, {.val = 447}, {.val = 407}, {.val = 375}, {.val = 348}, {.val = 326}, {.val = 309}, {.val = 296}, {.val = 289}, {.val = 284}, {.val = 284}, {.val = 288}, {.val = 294}, {.val = 306}, {.val = 322}, {.val = 342}, {.val = 366}, {.val = 395}, {.val = 427}, {.val = 464}, {.val = 464}, {.val = 517}, {.val = 471}, {.val = 432}, {.val = 397}, {.val = 369}, {.val = 348}, {.val = 330}, {.val = 316}, {.val = 309}, {.val = 304}, {.val = 304}, {.val = 308}, {.val = 315}, {.val = 327}, {.val = 344}, {.val = 363}, {.val = 388}, {.val = 417}, {.val = 449}, {.val = 485}, {.val = 485}, {.val = 533}, {.val = 490}, {.val = 450}, {.val = 415}, {.val = 386}, {.val = 363}, {.val = 347}, {.val = 332}, {.val = 324}, {.val = 320}, {.val = 319}, {.val = 323}, {.val = 330}, {.val = 344}, {.val = 358}, {.val = 378}, {.val = 403}, {.val = 432}, {.val = 464}, {.val = 502}, {.val = 502}, {.val = 533}, {.val = 490}, {.val = 450}, {.val = 415}, {.val = 386}, {.val = 363}, {.val = 347}, {.val = 332}, {.val = 324}, {.val = 320}, {.val = 319}, {.val = 323}, {.val = 330}, {.val = 344}, {.val = 358}, {.val = 378}, {.val = 403}, {.val = 432}, {.val = 464}, {.val = 502}, {.val = 502}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_6746_config[] = {
    {.val = 703}, {.val = 626}, {.val = 558}, {.val = 500}, {.val = 452}, {.val = 414}, {.val = 386}, {.val = 367}, {.val = 359}, {.val = 352}, {.val = 346}, {.val = 354}, {.val = 367}, {.val = 391}, {.val = 420}, {.val = 458}, {.val = 507}, {.val = 565}, {.val = 630}, {.val = 702}, {.val = 702}, {.val = 655}, {.val = 578}, {.val = 513}, {.val = 457}, {.val = 412}, {.val = 376}, {.val = 349}, {.val = 332}, {.val = 324}, {.val = 318}, {.val = 311}, {.val = 318}, {.val = 331}, {.val = 351}, {.val = 380}, {.val = 418}, {.val = 465}, {.val = 520}, {.val = 586}, {.val = 659}, {.val = 659}, {.val = 616}, {.val = 543}, {.val = 479}, {.val = 425}, {.val = 381}, {.val = 347}, {.val = 321}, {.val = 303}, {.val = 294}, {.val = 288}, {.val = 287}, {.val = 293}, {.val = 305}, {.val = 324}, {.val = 351}, {.val = 387}, {.val = 432}, {.val = 486}, {.val = 550}, {.val = 623}, {.val = 623}, {.val = 589}, {.val = 517}, {.val = 454}, {.val = 402}, {.val = 360}, {.val = 326}, {.val = 303}, {.val = 285}, {.val = 275}, {.val = 270}, {.val = 270}, {.val = 276}, {.val = 286}, {.val = 305}, {.val = 331}, {.val = 366}, {.val = 409}, {.val = 462}, {.val = 526}, {.val = 598}, {.val = 598}, {.val = 573}, {.val = 500}, {.val = 439}, {.val = 388}, {.val = 347}, {.val = 315}, {.val = 291}, {.val = 274}, {.val = 264}, {.val = 260}, {.val = 260}, {.val = 265}, {.val = 275}, {.val = 293}, {.val = 318}, {.val = 353}, {.val = 396}, {.val = 449}, {.val = 511}, {.val = 581}, {.val = 581}, {.val = 566}, {.val = 495}, {.val = 433}, {.val = 382}, {.val = 341}, {.val = 309}, {.val = 286}, {.val = 270}, {.val = 260}, {.val = 255}, {.val = 255}, {.val = 260}, {.val = 271}, {.val = 289}, {.val = 314}, {.val = 347}, {.val = 390}, {.val = 444}, {.val = 504}, {.val = 576}, {.val = 576}, {.val = 568}, {.val = 497}, {.val = 436}, {.val = 384}, {.val = 344}, {.val = 312}, {.val = 288}, {.val = 272}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 261}, {.val = 272}, {.val = 290}, {.val = 316}, {.val = 350}, {.val = 393}, {.val = 446}, {.val = 507}, {.val = 578}, {.val = 578}, {.val = 580}, {.val = 509}, {.val = 446}, {.val = 395}, {.val = 353}, {.val = 321}, {.val = 296}, {.val = 279}, {.val = 268}, {.val = 263}, {.val = 263}, {.val = 269}, {.val = 281}, {.val = 299}, {.val = 326}, {.val = 360}, {.val = 403}, {.val = 457}, {.val = 519}, {.val = 590}, {.val = 590}, {.val = 600}, {.val = 528}, {.val = 466}, {.val = 413}, {.val = 370}, {.val = 337}, {.val = 311}, {.val = 294}, {.val = 282}, {.val = 277}, {.val = 277}, {.val = 284}, {.val = 295}, {.val = 315}, {.val = 342}, {.val = 378}, {.val = 423}, {.val = 478}, {.val = 540}, {.val = 611}, {.val = 611}, {.val = 632}, {.val = 558}, {.val = 494}, {.val = 440}, {.val = 397}, {.val = 361}, {.val = 336}, {.val = 316}, {.val = 305}, {.val = 299}, {.val = 299}, {.val = 306}, {.val = 319}, {.val = 340}, {.val = 368}, {.val = 406}, {.val = 452}, {.val = 508}, {.val = 571}, {.val = 644}, {.val = 644}, {.val = 672}, {.val = 598}, {.val = 534}, {.val = 478}, {.val = 433}, {.val = 397}, {.val = 368}, {.val = 348}, {.val = 336}, {.val = 329}, {.val = 330}, {.val = 338}, {.val = 351}, {.val = 374}, {.val = 404}, {.val = 442}, {.val = 490}, {.val = 546}, {.val = 610}, {.val = 683}, {.val = 683}, {.val = 703}, {.val = 626}, {.val = 564}, {.val = 507}, {.val = 458}, {.val = 422}, {.val = 394}, {.val = 373}, {.val = 361}, {.val = 354}, {.val = 354}, {.val = 362}, {.val = 377}, {.val = 401}, {.val = 431}, {.val = 471}, {.val = 518}, {.val = 576}, {.val = 637}, {.val = 716}, {.val = 716}, {.val = 703}, {.val = 626}, {.val = 564}, {.val = 507}, {.val = 458}, {.val = 422}, {.val = 394}, {.val = 373}, {.val = 361}, {.val = 354}, {.val = 354}, {.val = 362}, {.val = 377}, {.val = 401}, {.val = 431}, {.val = 471}, {.val = 518}, {.val = 576}, {.val = 637}, {.val = 716}, {.val = 716}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_6746_config[] = {
    {.val = 558}, {.val = 510}, {.val = 465}, {.val = 430}, {.val = 399}, {.val = 373}, {.val = 354}, {.val = 340}, {.val = 337}, {.val = 332}, {.val = 324}, {.val = 328}, {.val = 337}, {.val = 352}, {.val = 371}, {.val = 393}, {.val = 423}, {.val = 457}, {.val = 497}, {.val = 541}, {.val = 541}, {.val = 529}, {.val = 480}, {.val = 437}, {.val = 401}, {.val = 371}, {.val = 346}, {.val = 327}, {.val = 315}, {.val = 311}, {.val = 306}, {.val = 300}, {.val = 304}, {.val = 312}, {.val = 325}, {.val = 344}, {.val = 367}, {.val = 396}, {.val = 430}, {.val = 469}, {.val = 514}, {.val = 514}, {.val = 504}, {.val = 456}, {.val = 414}, {.val = 378}, {.val = 349}, {.val = 325}, {.val = 307}, {.val = 294}, {.val = 287}, {.val = 283}, {.val = 281}, {.val = 285}, {.val = 292}, {.val = 305}, {.val = 323}, {.val = 346}, {.val = 374}, {.val = 408}, {.val = 448}, {.val = 492}, {.val = 492}, {.val = 486}, {.val = 438}, {.val = 397}, {.val = 362}, {.val = 333}, {.val = 310}, {.val = 293}, {.val = 280}, {.val = 272}, {.val = 269}, {.val = 268}, {.val = 272}, {.val = 279}, {.val = 291}, {.val = 308}, {.val = 330}, {.val = 359}, {.val = 393}, {.val = 432}, {.val = 477}, {.val = 477}, {.val = 476}, {.val = 428}, {.val = 386}, {.val = 351}, {.val = 323}, {.val = 301}, {.val = 283}, {.val = 271}, {.val = 264}, {.val = 260}, {.val = 260}, {.val = 263}, {.val = 270}, {.val = 282}, {.val = 299}, {.val = 321}, {.val = 349}, {.val = 383}, {.val = 422}, {.val = 467}, {.val = 467}, {.val = 470}, {.val = 422}, {.val = 382}, {.val = 347}, {.val = 319}, {.val = 297}, {.val = 280}, {.val = 267}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 266}, {.val = 278}, {.val = 295}, {.val = 317}, {.val = 345}, {.val = 379}, {.val = 418}, {.val = 462}, {.val = 462}, {.val = 470}, {.val = 423}, {.val = 383}, {.val = 348}, {.val = 320}, {.val = 297}, {.val = 280}, {.val = 268}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 267}, {.val = 278}, {.val = 296}, {.val = 318}, {.val = 347}, {.val = 380}, {.val = 420}, {.val = 464}, {.val = 464}, {.val = 476}, {.val = 430}, {.val = 389}, {.val = 354}, {.val = 325}, {.val = 303}, {.val = 285}, {.val = 273}, {.val = 265}, {.val = 261}, {.val = 261}, {.val = 265}, {.val = 272}, {.val = 285}, {.val = 302}, {.val = 325}, {.val = 353}, {.val = 387}, {.val = 427}, {.val = 471}, {.val = 471}, {.val = 488}, {.val = 442}, {.val = 401}, {.val = 366}, {.val = 337}, {.val = 314}, {.val = 296}, {.val = 283}, {.val = 275}, {.val = 271}, {.val = 271}, {.val = 275}, {.val = 283}, {.val = 297}, {.val = 314}, {.val = 337}, {.val = 366}, {.val = 400}, {.val = 439}, {.val = 484}, {.val = 484}, {.val = 508}, {.val = 460}, {.val = 418}, {.val = 383}, {.val = 354}, {.val = 331}, {.val = 312}, {.val = 299}, {.val = 290}, {.val = 286}, {.val = 286}, {.val = 290}, {.val = 299}, {.val = 313}, {.val = 331}, {.val = 355}, {.val = 384}, {.val = 418}, {.val = 458}, {.val = 502}, {.val = 502}, {.val = 531}, {.val = 484}, {.val = 442}, {.val = 406}, {.val = 377}, {.val = 352}, {.val = 334}, {.val = 320}, {.val = 311}, {.val = 307}, {.val = 307}, {.val = 311}, {.val = 321}, {.val = 334}, {.val = 353}, {.val = 377}, {.val = 407}, {.val = 442}, {.val = 482}, {.val = 526}, {.val = 526}, {.val = 549}, {.val = 503}, {.val = 459}, {.val = 423}, {.val = 395}, {.val = 370}, {.val = 351}, {.val = 337}, {.val = 328}, {.val = 323}, {.val = 323}, {.val = 328}, {.val = 337}, {.val = 352}, {.val = 371}, {.val = 394}, {.val = 425}, {.val = 460}, {.val = 499}, {.val = 543}, {.val = 543}, {.val = 549}, {.val = 503}, {.val = 459}, {.val = 423}, {.val = 395}, {.val = 370}, {.val = 351}, {.val = 337}, {.val = 328}, {.val = 323}, {.val = 323}, {.val = 328}, {.val = 337}, {.val = 352}, {.val = 371}, {.val = 394}, {.val = 425}, {.val = 460}, {.val = 499}, {.val = 543}, {.val = 543}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_6746_config[] = {
    {.val = 544}, {.val = 496}, {.val = 456}, {.val = 420}, {.val = 390}, {.val = 366}, {.val = 347}, {.val = 334}, {.val = 332}, {.val = 326}, {.val = 319}, {.val = 324}, {.val = 332}, {.val = 347}, {.val = 366}, {.val = 388}, {.val = 417}, {.val = 450}, {.val = 488}, {.val = 531}, {.val = 531}, {.val = 517}, {.val = 471}, {.val = 429}, {.val = 395}, {.val = 365}, {.val = 342}, {.val = 323}, {.val = 311}, {.val = 306}, {.val = 303}, {.val = 296}, {.val = 299}, {.val = 308}, {.val = 321}, {.val = 339}, {.val = 362}, {.val = 391}, {.val = 423}, {.val = 462}, {.val = 506}, {.val = 506}, {.val = 497}, {.val = 451}, {.val = 410}, {.val = 374}, {.val = 346}, {.val = 322}, {.val = 305}, {.val = 291}, {.val = 284}, {.val = 280}, {.val = 278}, {.val = 282}, {.val = 290}, {.val = 302}, {.val = 319}, {.val = 342}, {.val = 370}, {.val = 404}, {.val = 442}, {.val = 485}, {.val = 485}, {.val = 483}, {.val = 436}, {.val = 395}, {.val = 361}, {.val = 332}, {.val = 309}, {.val = 291}, {.val = 279}, {.val = 271}, {.val = 267}, {.val = 267}, {.val = 269}, {.val = 277}, {.val = 289}, {.val = 306}, {.val = 328}, {.val = 356}, {.val = 389}, {.val = 427}, {.val = 470}, {.val = 470}, {.val = 476}, {.val = 428}, {.val = 387}, {.val = 352}, {.val = 323}, {.val = 301}, {.val = 283}, {.val = 271}, {.val = 263}, {.val = 259}, {.val = 259}, {.val = 262}, {.val = 269}, {.val = 280}, {.val = 297}, {.val = 320}, {.val = 347}, {.val = 380}, {.val = 418}, {.val = 460}, {.val = 460}, {.val = 473}, {.val = 425}, {.val = 384}, {.val = 350}, {.val = 321}, {.val = 297}, {.val = 280}, {.val = 267}, {.val = 260}, {.val = 256}, {.val = 255}, {.val = 258}, {.val = 265}, {.val = 277}, {.val = 294}, {.val = 316}, {.val = 343}, {.val = 376}, {.val = 415}, {.val = 458}, {.val = 458}, {.val = 476}, {.val = 429}, {.val = 387}, {.val = 352}, {.val = 322}, {.val = 300}, {.val = 282}, {.val = 269}, {.val = 261}, {.val = 257}, {.val = 256}, {.val = 259}, {.val = 266}, {.val = 278}, {.val = 295}, {.val = 318}, {.val = 345}, {.val = 379}, {.val = 417}, {.val = 460}, {.val = 460}, {.val = 485}, {.val = 437}, {.val = 395}, {.val = 360}, {.val = 330}, {.val = 307}, {.val = 288}, {.val = 275}, {.val = 266}, {.val = 262}, {.val = 261}, {.val = 265}, {.val = 272}, {.val = 285}, {.val = 302}, {.val = 325}, {.val = 353}, {.val = 387}, {.val = 425}, {.val = 468}, {.val = 468}, {.val = 500}, {.val = 451}, {.val = 410}, {.val = 374}, {.val = 343}, {.val = 319}, {.val = 300}, {.val = 286}, {.val = 277}, {.val = 272}, {.val = 272}, {.val = 275}, {.val = 284}, {.val = 297}, {.val = 315}, {.val = 338}, {.val = 366}, {.val = 400}, {.val = 438}, {.val = 482}, {.val = 482}, {.val = 520}, {.val = 472}, {.val = 430}, {.val = 393}, {.val = 362}, {.val = 337}, {.val = 318}, {.val = 303}, {.val = 294}, {.val = 288}, {.val = 288}, {.val = 292}, {.val = 301}, {.val = 315}, {.val = 333}, {.val = 357}, {.val = 385}, {.val = 419}, {.val = 457}, {.val = 501}, {.val = 501}, {.val = 549}, {.val = 499}, {.val = 456}, {.val = 418}, {.val = 387}, {.val = 361}, {.val = 341}, {.val = 326}, {.val = 316}, {.val = 311}, {.val = 310}, {.val = 315}, {.val = 324}, {.val = 337}, {.val = 356}, {.val = 380}, {.val = 409}, {.val = 443}, {.val = 482}, {.val = 525}, {.val = 525}, {.val = 569}, {.val = 518}, {.val = 476}, {.val = 438}, {.val = 405}, {.val = 379}, {.val = 359}, {.val = 344}, {.val = 334}, {.val = 328}, {.val = 327}, {.val = 333}, {.val = 342}, {.val = 356}, {.val = 375}, {.val = 399}, {.val = 426}, {.val = 461}, {.val = 500}, {.val = 543}, {.val = 543}, {.val = 569}, {.val = 518}, {.val = 476}, {.val = 438}, {.val = 405}, {.val = 379}, {.val = 359}, {.val = 344}, {.val = 334}, {.val = 328}, {.val = 327}, {.val = 333}, {.val = 342}, {.val = 356}, {.val = 375}, {.val = 399}, {.val = 426}, {.val = 461}, {.val = 500}, {.val = 543}, {.val = 543}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_6746_config[] = {
    {.val = 507}, {.val = 466}, {.val = 430}, {.val = 399}, {.val = 374}, {.val = 353}, {.val = 338}, {.val = 327}, {.val = 325}, {.val = 319}, {.val = 313}, {.val = 315}, {.val = 322}, {.val = 334}, {.val = 349}, {.val = 367}, {.val = 390}, {.val = 418}, {.val = 450}, {.val = 485}, {.val = 485}, {.val = 485}, {.val = 444}, {.val = 407}, {.val = 377}, {.val = 352}, {.val = 331}, {.val = 315}, {.val = 306}, {.val = 301}, {.val = 299}, {.val = 292}, {.val = 294}, {.val = 301}, {.val = 312}, {.val = 326}, {.val = 345}, {.val = 369}, {.val = 396}, {.val = 429}, {.val = 464}, {.val = 464}, {.val = 469}, {.val = 426}, {.val = 391}, {.val = 360}, {.val = 335}, {.val = 314}, {.val = 298}, {.val = 288}, {.val = 282}, {.val = 278}, {.val = 276}, {.val = 278}, {.val = 285}, {.val = 295}, {.val = 309}, {.val = 328}, {.val = 352}, {.val = 380}, {.val = 412}, {.val = 447}, {.val = 447}, {.val = 457}, {.val = 415}, {.val = 379}, {.val = 348}, {.val = 323}, {.val = 302}, {.val = 287}, {.val = 276}, {.val = 269}, {.val = 266}, {.val = 266}, {.val = 268}, {.val = 274}, {.val = 283}, {.val = 298}, {.val = 316}, {.val = 340}, {.val = 368}, {.val = 400}, {.val = 435}, {.val = 435}, {.val = 450}, {.val = 408}, {.val = 372}, {.val = 341}, {.val = 316}, {.val = 295}, {.val = 280}, {.val = 269}, {.val = 262}, {.val = 260}, {.val = 259}, {.val = 261}, {.val = 267}, {.val = 276}, {.val = 290}, {.val = 309}, {.val = 333}, {.val = 361}, {.val = 392}, {.val = 428}, {.val = 428}, {.val = 448}, {.val = 406}, {.val = 369}, {.val = 338}, {.val = 313}, {.val = 292}, {.val = 277}, {.val = 266}, {.val = 259}, {.val = 256}, {.val = 256}, {.val = 258}, {.val = 263}, {.val = 273}, {.val = 287}, {.val = 306}, {.val = 330}, {.val = 358}, {.val = 390}, {.val = 426}, {.val = 426}, {.val = 451}, {.val = 408}, {.val = 371}, {.val = 340}, {.val = 315}, {.val = 294}, {.val = 278}, {.val = 267}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 264}, {.val = 274}, {.val = 289}, {.val = 307}, {.val = 332}, {.val = 360}, {.val = 392}, {.val = 428}, {.val = 428}, {.val = 458}, {.val = 415}, {.val = 378}, {.val = 347}, {.val = 321}, {.val = 299}, {.val = 284}, {.val = 272}, {.val = 265}, {.val = 261}, {.val = 260}, {.val = 263}, {.val = 270}, {.val = 280}, {.val = 295}, {.val = 314}, {.val = 338}, {.val = 367}, {.val = 399}, {.val = 435}, {.val = 435}, {.val = 472}, {.val = 428}, {.val = 390}, {.val = 359}, {.val = 332}, {.val = 310}, {.val = 294}, {.val = 282}, {.val = 274}, {.val = 270}, {.val = 270}, {.val = 272}, {.val = 279}, {.val = 291}, {.val = 306}, {.val = 325}, {.val = 350}, {.val = 377}, {.val = 410}, {.val = 447}, {.val = 447}, {.val = 491}, {.val = 447}, {.val = 407}, {.val = 375}, {.val = 348}, {.val = 326}, {.val = 309}, {.val = 296}, {.val = 288}, {.val = 284}, {.val = 284}, {.val = 287}, {.val = 294}, {.val = 306}, {.val = 322}, {.val = 341}, {.val = 366}, {.val = 394}, {.val = 426}, {.val = 463}, {.val = 463}, {.val = 516}, {.val = 470}, {.val = 431}, {.val = 397}, {.val = 369}, {.val = 348}, {.val = 329}, {.val = 317}, {.val = 308}, {.val = 304}, {.val = 303}, {.val = 307}, {.val = 314}, {.val = 327}, {.val = 343}, {.val = 362}, {.val = 387}, {.val = 415}, {.val = 446}, {.val = 484}, {.val = 484}, {.val = 533}, {.val = 490}, {.val = 448}, {.val = 414}, {.val = 385}, {.val = 363}, {.val = 346}, {.val = 331}, {.val = 324}, {.val = 321}, {.val = 318}, {.val = 323}, {.val = 330}, {.val = 342}, {.val = 359}, {.val = 377}, {.val = 402}, {.val = 430}, {.val = 462}, {.val = 499}, {.val = 499}, {.val = 533}, {.val = 490}, {.val = 448}, {.val = 414}, {.val = 385}, {.val = 363}, {.val = 346}, {.val = 331}, {.val = 324}, {.val = 321}, {.val = 318}, {.val = 323}, {.val = 330}, {.val = 342}, {.val = 359}, {.val = 377}, {.val = 402}, {.val = 430}, {.val = 462}, {.val = 499}, {.val = 499}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_7378_config[] = {
    {.val = 684}, {.val = 613}, {.val = 547}, {.val = 491}, {.val = 447}, {.val = 409}, {.val = 382}, {.val = 364}, {.val = 358}, {.val = 351}, {.val = 343}, {.val = 351}, {.val = 364}, {.val = 387}, {.val = 415}, {.val = 452}, {.val = 499}, {.val = 553}, {.val = 615}, {.val = 684}, {.val = 684}, {.val = 640}, {.val = 567}, {.val = 505}, {.val = 452}, {.val = 408}, {.val = 373}, {.val = 347}, {.val = 329}, {.val = 323}, {.val = 317}, {.val = 311}, {.val = 317}, {.val = 329}, {.val = 349}, {.val = 377}, {.val = 414}, {.val = 456}, {.val = 510}, {.val = 574}, {.val = 644}, {.val = 644}, {.val = 605}, {.val = 533}, {.val = 473}, {.val = 420}, {.val = 378}, {.val = 344}, {.val = 319}, {.val = 301}, {.val = 293}, {.val = 287}, {.val = 286}, {.val = 292}, {.val = 304}, {.val = 321}, {.val = 348}, {.val = 383}, {.val = 425}, {.val = 479}, {.val = 539}, {.val = 608}, {.val = 608}, {.val = 578}, {.val = 508}, {.val = 448}, {.val = 398}, {.val = 357}, {.val = 325}, {.val = 301}, {.val = 285}, {.val = 274}, {.val = 270}, {.val = 270}, {.val = 275}, {.val = 286}, {.val = 303}, {.val = 329}, {.val = 362}, {.val = 404}, {.val = 457}, {.val = 517}, {.val = 585}, {.val = 585}, {.val = 563}, {.val = 493}, {.val = 434}, {.val = 385}, {.val = 344}, {.val = 313}, {.val = 290}, {.val = 274}, {.val = 264}, {.val = 259}, {.val = 259}, {.val = 265}, {.val = 274}, {.val = 292}, {.val = 317}, {.val = 350}, {.val = 391}, {.val = 442}, {.val = 502}, {.val = 568}, {.val = 568}, {.val = 555}, {.val = 485}, {.val = 427}, {.val = 379}, {.val = 339}, {.val = 308}, {.val = 285}, {.val = 269}, {.val = 260}, {.val = 255}, {.val = 255}, {.val = 260}, {.val = 270}, {.val = 287}, {.val = 312}, {.val = 345}, {.val = 386}, {.val = 437}, {.val = 496}, {.val = 562}, {.val = 562}, {.val = 556}, {.val = 488}, {.val = 429}, {.val = 381}, {.val = 341}, {.val = 310}, {.val = 287}, {.val = 271}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 261}, {.val = 272}, {.val = 289}, {.val = 313}, {.val = 346}, {.val = 388}, {.val = 437}, {.val = 497}, {.val = 565}, {.val = 565}, {.val = 566}, {.val = 498}, {.val = 440}, {.val = 390}, {.val = 350}, {.val = 318}, {.val = 294}, {.val = 278}, {.val = 268}, {.val = 262}, {.val = 262}, {.val = 268}, {.val = 279}, {.val = 297}, {.val = 323}, {.val = 356}, {.val = 398}, {.val = 450}, {.val = 508}, {.val = 577}, {.val = 577}, {.val = 587}, {.val = 518}, {.val = 457}, {.val = 408}, {.val = 366}, {.val = 334}, {.val = 310}, {.val = 293}, {.val = 281}, {.val = 276}, {.val = 277}, {.val = 282}, {.val = 294}, {.val = 314}, {.val = 340}, {.val = 374}, {.val = 417}, {.val = 469}, {.val = 529}, {.val = 597}, {.val = 597}, {.val = 617}, {.val = 547}, {.val = 486}, {.val = 435}, {.val = 392}, {.val = 358}, {.val = 333}, {.val = 314}, {.val = 303}, {.val = 298}, {.val = 298}, {.val = 304}, {.val = 317}, {.val = 337}, {.val = 364}, {.val = 400}, {.val = 445}, {.val = 498}, {.val = 558}, {.val = 626}, {.val = 626}, {.val = 654}, {.val = 586}, {.val = 524}, {.val = 470}, {.val = 427}, {.val = 391}, {.val = 365}, {.val = 346}, {.val = 334}, {.val = 327}, {.val = 328}, {.val = 335}, {.val = 349}, {.val = 370}, {.val = 398}, {.val = 435}, {.val = 481}, {.val = 535}, {.val = 597}, {.val = 666}, {.val = 666}, {.val = 688}, {.val = 615}, {.val = 552}, {.val = 499}, {.val = 455}, {.val = 418}, {.val = 390}, {.val = 370}, {.val = 358}, {.val = 352}, {.val = 352}, {.val = 359}, {.val = 375}, {.val = 396}, {.val = 425}, {.val = 462}, {.val = 506}, {.val = 564}, {.val = 622}, {.val = 696}, {.val = 696}, {.val = 688}, {.val = 615}, {.val = 552}, {.val = 499}, {.val = 455}, {.val = 418}, {.val = 390}, {.val = 370}, {.val = 358}, {.val = 352}, {.val = 352}, {.val = 359}, {.val = 375}, {.val = 396}, {.val = 425}, {.val = 462}, {.val = 506}, {.val = 564}, {.val = 622}, {.val = 696}, {.val = 696}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_7378_config[] = {
    {.val = 557}, {.val = 508}, {.val = 465}, {.val = 429}, {.val = 398}, {.val = 372}, {.val = 353}, {.val = 340}, {.val = 336}, {.val = 331}, {.val = 324}, {.val = 328}, {.val = 337}, {.val = 351}, {.val = 370}, {.val = 392}, {.val = 421}, {.val = 456}, {.val = 495}, {.val = 538}, {.val = 538}, {.val = 527}, {.val = 478}, {.val = 436}, {.val = 400}, {.val = 370}, {.val = 346}, {.val = 327}, {.val = 315}, {.val = 311}, {.val = 306}, {.val = 300}, {.val = 303}, {.val = 312}, {.val = 324}, {.val = 343}, {.val = 366}, {.val = 395}, {.val = 428}, {.val = 469}, {.val = 512}, {.val = 512}, {.val = 502}, {.val = 454}, {.val = 413}, {.val = 378}, {.val = 348}, {.val = 325}, {.val = 307}, {.val = 294}, {.val = 287}, {.val = 282}, {.val = 281}, {.val = 285}, {.val = 292}, {.val = 305}, {.val = 322}, {.val = 344}, {.val = 373}, {.val = 407}, {.val = 446}, {.val = 491}, {.val = 491}, {.val = 485}, {.val = 437}, {.val = 396}, {.val = 361}, {.val = 333}, {.val = 310}, {.val = 293}, {.val = 280}, {.val = 272}, {.val = 268}, {.val = 268}, {.val = 271}, {.val = 279}, {.val = 290}, {.val = 307}, {.val = 329}, {.val = 358}, {.val = 391}, {.val = 431}, {.val = 475}, {.val = 475}, {.val = 474}, {.val = 427}, {.val = 386}, {.val = 351}, {.val = 322}, {.val = 301}, {.val = 283}, {.val = 271}, {.val = 264}, {.val = 260}, {.val = 260}, {.val = 263}, {.val = 270}, {.val = 282}, {.val = 299}, {.val = 321}, {.val = 349}, {.val = 382}, {.val = 421}, {.val = 466}, {.val = 466}, {.val = 469}, {.val = 421}, {.val = 380}, {.val = 346}, {.val = 319}, {.val = 296}, {.val = 279}, {.val = 267}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 266}, {.val = 278}, {.val = 295}, {.val = 316}, {.val = 344}, {.val = 378}, {.val = 417}, {.val = 461}, {.val = 461}, {.val = 469}, {.val = 422}, {.val = 382}, {.val = 347}, {.val = 319}, {.val = 297}, {.val = 280}, {.val = 268}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 266}, {.val = 278}, {.val = 295}, {.val = 317}, {.val = 346}, {.val = 379}, {.val = 419}, {.val = 462}, {.val = 462}, {.val = 475}, {.val = 428}, {.val = 388}, {.val = 353}, {.val = 324}, {.val = 303}, {.val = 285}, {.val = 272}, {.val = 265}, {.val = 261}, {.val = 261}, {.val = 264}, {.val = 272}, {.val = 284}, {.val = 302}, {.val = 324}, {.val = 353}, {.val = 386}, {.val = 425}, {.val = 469}, {.val = 469}, {.val = 487}, {.val = 440}, {.val = 400}, {.val = 365}, {.val = 337}, {.val = 314}, {.val = 296}, {.val = 283}, {.val = 275}, {.val = 271}, {.val = 271}, {.val = 274}, {.val = 282}, {.val = 296}, {.val = 313}, {.val = 337}, {.val = 365}, {.val = 399}, {.val = 438}, {.val = 482}, {.val = 482}, {.val = 505}, {.val = 458}, {.val = 417}, {.val = 382}, {.val = 353}, {.val = 330}, {.val = 312}, {.val = 298}, {.val = 290}, {.val = 286}, {.val = 285}, {.val = 290}, {.val = 298}, {.val = 312}, {.val = 330}, {.val = 354}, {.val = 383}, {.val = 417}, {.val = 456}, {.val = 500}, {.val = 500}, {.val = 528}, {.val = 482}, {.val = 441}, {.val = 405}, {.val = 376}, {.val = 352}, {.val = 333}, {.val = 319}, {.val = 310}, {.val = 306}, {.val = 306}, {.val = 311}, {.val = 320}, {.val = 334}, {.val = 353}, {.val = 376}, {.val = 406}, {.val = 440}, {.val = 479}, {.val = 523}, {.val = 523}, {.val = 547}, {.val = 501}, {.val = 457}, {.val = 423}, {.val = 393}, {.val = 369}, {.val = 349}, {.val = 335}, {.val = 327}, {.val = 322}, {.val = 322}, {.val = 328}, {.val = 337}, {.val = 351}, {.val = 370}, {.val = 394}, {.val = 424}, {.val = 457}, {.val = 496}, {.val = 540}, {.val = 540}, {.val = 547}, {.val = 501}, {.val = 457}, {.val = 423}, {.val = 393}, {.val = 369}, {.val = 349}, {.val = 335}, {.val = 327}, {.val = 322}, {.val = 322}, {.val = 328}, {.val = 337}, {.val = 351}, {.val = 370}, {.val = 394}, {.val = 424}, {.val = 457}, {.val = 496}, {.val = 540}, {.val = 540}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_7378_config[] = {
    {.val = 540}, {.val = 494}, {.val = 453}, {.val = 419}, {.val = 389}, {.val = 365}, {.val = 347}, {.val = 335}, {.val = 332}, {.val = 326}, {.val = 319}, {.val = 323}, {.val = 332}, {.val = 347}, {.val = 365}, {.val = 387}, {.val = 415}, {.val = 448}, {.val = 485}, {.val = 527}, {.val = 527}, {.val = 516}, {.val = 468}, {.val = 428}, {.val = 393}, {.val = 365}, {.val = 341}, {.val = 323}, {.val = 311}, {.val = 306}, {.val = 302}, {.val = 296}, {.val = 299}, {.val = 308}, {.val = 321}, {.val = 338}, {.val = 361}, {.val = 389}, {.val = 421}, {.val = 459}, {.val = 502}, {.val = 502}, {.val = 495}, {.val = 448}, {.val = 408}, {.val = 374}, {.val = 345}, {.val = 322}, {.val = 304}, {.val = 291}, {.val = 285}, {.val = 280}, {.val = 278}, {.val = 281}, {.val = 289}, {.val = 302}, {.val = 319}, {.val = 341}, {.val = 369}, {.val = 402}, {.val = 439}, {.val = 481}, {.val = 481}, {.val = 481}, {.val = 434}, {.val = 395}, {.val = 360}, {.val = 331}, {.val = 309}, {.val = 291}, {.val = 279}, {.val = 271}, {.val = 267}, {.val = 266}, {.val = 269}, {.val = 276}, {.val = 288}, {.val = 305}, {.val = 327}, {.val = 355}, {.val = 387}, {.val = 424}, {.val = 468}, {.val = 468}, {.val = 473}, {.val = 426}, {.val = 386}, {.val = 351}, {.val = 323}, {.val = 301}, {.val = 283}, {.val = 271}, {.val = 263}, {.val = 259}, {.val = 259}, {.val = 262}, {.val = 268}, {.val = 280}, {.val = 297}, {.val = 319}, {.val = 346}, {.val = 379}, {.val = 416}, {.val = 459}, {.val = 459}, {.val = 470}, {.val = 424}, {.val = 383}, {.val = 348}, {.val = 320}, {.val = 297}, {.val = 280}, {.val = 267}, {.val = 260}, {.val = 255}, {.val = 255}, {.val = 258}, {.val = 265}, {.val = 277}, {.val = 293}, {.val = 315}, {.val = 343}, {.val = 375}, {.val = 412}, {.val = 456}, {.val = 456}, {.val = 473}, {.val = 427}, {.val = 386}, {.val = 351}, {.val = 322}, {.val = 299}, {.val = 281}, {.val = 269}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 266}, {.val = 278}, {.val = 295}, {.val = 317}, {.val = 345}, {.val = 377}, {.val = 415}, {.val = 457}, {.val = 457}, {.val = 482}, {.val = 436}, {.val = 394}, {.val = 359}, {.val = 329}, {.val = 306}, {.val = 288}, {.val = 275}, {.val = 266}, {.val = 262}, {.val = 261}, {.val = 265}, {.val = 272}, {.val = 284}, {.val = 302}, {.val = 324}, {.val = 352}, {.val = 386}, {.val = 423}, {.val = 466}, {.val = 466}, {.val = 498}, {.val = 450}, {.val = 408}, {.val = 373}, {.val = 343}, {.val = 318}, {.val = 299}, {.val = 286}, {.val = 277}, {.val = 272}, {.val = 272}, {.val = 275}, {.val = 283}, {.val = 296}, {.val = 314}, {.val = 337}, {.val = 365}, {.val = 398}, {.val = 436}, {.val = 478}, {.val = 478}, {.val = 519}, {.val = 471}, {.val = 428}, {.val = 392}, {.val = 362}, {.val = 336}, {.val = 317}, {.val = 303}, {.val = 293}, {.val = 288}, {.val = 288}, {.val = 291}, {.val = 300}, {.val = 314}, {.val = 332}, {.val = 355}, {.val = 384}, {.val = 417}, {.val = 455}, {.val = 498}, {.val = 498}, {.val = 546}, {.val = 497}, {.val = 454}, {.val = 417}, {.val = 387}, {.val = 361}, {.val = 341}, {.val = 326}, {.val = 316}, {.val = 311}, {.val = 310}, {.val = 314}, {.val = 323}, {.val = 337}, {.val = 356}, {.val = 379}, {.val = 408}, {.val = 442}, {.val = 480}, {.val = 523}, {.val = 523}, {.val = 567}, {.val = 517}, {.val = 475}, {.val = 437}, {.val = 404}, {.val = 379}, {.val = 359}, {.val = 343}, {.val = 334}, {.val = 328}, {.val = 327}, {.val = 332}, {.val = 341}, {.val = 355}, {.val = 373}, {.val = 397}, {.val = 426}, {.val = 460}, {.val = 498}, {.val = 540}, {.val = 540}, {.val = 567}, {.val = 517}, {.val = 475}, {.val = 437}, {.val = 404}, {.val = 379}, {.val = 359}, {.val = 343}, {.val = 334}, {.val = 328}, {.val = 327}, {.val = 332}, {.val = 341}, {.val = 355}, {.val = 373}, {.val = 397}, {.val = 426}, {.val = 460}, {.val = 498}, {.val = 540}, {.val = 540}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_7378_config[] = {
    {.val = 506}, {.val = 464}, {.val = 428}, {.val = 399}, {.val = 373}, {.val = 353}, {.val = 337}, {.val = 326}, {.val = 325}, {.val = 320}, {.val = 312}, {.val = 315}, {.val = 322}, {.val = 333}, {.val = 349}, {.val = 366}, {.val = 389}, {.val = 417}, {.val = 449}, {.val = 484}, {.val = 484}, {.val = 485}, {.val = 443}, {.val = 407}, {.val = 376}, {.val = 351}, {.val = 331}, {.val = 315}, {.val = 305}, {.val = 301}, {.val = 298}, {.val = 291}, {.val = 294}, {.val = 301}, {.val = 311}, {.val = 326}, {.val = 345}, {.val = 368}, {.val = 395}, {.val = 427}, {.val = 462}, {.val = 462}, {.val = 467}, {.val = 427}, {.val = 390}, {.val = 360}, {.val = 335}, {.val = 314}, {.val = 298}, {.val = 287}, {.val = 281}, {.val = 277}, {.val = 276}, {.val = 279}, {.val = 285}, {.val = 295}, {.val = 309}, {.val = 327}, {.val = 351}, {.val = 378}, {.val = 410}, {.val = 447}, {.val = 447}, {.val = 456}, {.val = 415}, {.val = 378}, {.val = 348}, {.val = 322}, {.val = 302}, {.val = 287}, {.val = 276}, {.val = 269}, {.val = 266}, {.val = 266}, {.val = 268}, {.val = 274}, {.val = 283}, {.val = 297}, {.val = 316}, {.val = 339}, {.val = 367}, {.val = 398}, {.val = 434}, {.val = 434}, {.val = 450}, {.val = 408}, {.val = 371}, {.val = 340}, {.val = 315}, {.val = 295}, {.val = 279}, {.val = 269}, {.val = 262}, {.val = 259}, {.val = 259}, {.val = 261}, {.val = 267}, {.val = 275}, {.val = 290}, {.val = 309}, {.val = 332}, {.val = 360}, {.val = 392}, {.val = 427}, {.val = 427}, {.val = 449}, {.val = 406}, {.val = 369}, {.val = 338}, {.val = 313}, {.val = 292}, {.val = 276}, {.val = 266}, {.val = 259}, {.val = 256}, {.val = 256}, {.val = 258}, {.val = 263}, {.val = 273}, {.val = 287}, {.val = 305}, {.val = 329}, {.val = 357}, {.val = 389}, {.val = 425}, {.val = 425}, {.val = 449}, {.val = 407}, {.val = 371}, {.val = 339}, {.val = 314}, {.val = 293}, {.val = 278}, {.val = 267}, {.val = 259}, {.val = 256}, {.val = 256}, {.val = 258}, {.val = 264}, {.val = 274}, {.val = 288}, {.val = 307}, {.val = 331}, {.val = 359}, {.val = 391}, {.val = 427}, {.val = 427}, {.val = 458}, {.val = 415}, {.val = 378}, {.val = 347}, {.val = 321}, {.val = 300}, {.val = 284}, {.val = 272}, {.val = 265}, {.val = 261}, {.val = 260}, {.val = 263}, {.val = 269}, {.val = 279}, {.val = 295}, {.val = 313}, {.val = 337}, {.val = 366}, {.val = 398}, {.val = 433}, {.val = 433}, {.val = 472}, {.val = 427}, {.val = 390}, {.val = 358}, {.val = 332}, {.val = 310}, {.val = 294}, {.val = 282}, {.val = 273}, {.val = 270}, {.val = 269}, {.val = 272}, {.val = 279}, {.val = 290}, {.val = 305}, {.val = 325}, {.val = 349}, {.val = 377}, {.val = 409}, {.val = 446}, {.val = 446}, {.val = 491}, {.val = 446}, {.val = 407}, {.val = 374}, {.val = 347}, {.val = 326}, {.val = 309}, {.val = 296}, {.val = 288}, {.val = 284}, {.val = 283}, {.val = 287}, {.val = 294}, {.val = 305}, {.val = 322}, {.val = 341}, {.val = 365}, {.val = 393}, {.val = 425}, {.val = 461}, {.val = 461}, {.val = 515}, {.val = 469}, {.val = 429}, {.val = 397}, {.val = 369}, {.val = 347}, {.val = 329}, {.val = 316}, {.val = 308}, {.val = 303}, {.val = 303}, {.val = 307}, {.val = 314}, {.val = 326}, {.val = 342}, {.val = 362}, {.val = 385}, {.val = 414}, {.val = 446}, {.val = 483}, {.val = 483}, {.val = 533}, {.val = 487}, {.val = 448}, {.val = 413}, {.val = 384}, {.val = 363}, {.val = 344}, {.val = 332}, {.val = 324}, {.val = 318}, {.val = 319}, {.val = 322}, {.val = 329}, {.val = 342}, {.val = 358}, {.val = 376}, {.val = 401}, {.val = 430}, {.val = 462}, {.val = 498}, {.val = 498}, {.val = 533}, {.val = 487}, {.val = 448}, {.val = 413}, {.val = 384}, {.val = 363}, {.val = 344}, {.val = 332}, {.val = 324}, {.val = 318}, {.val = 319}, {.val = 322}, {.val = 329}, {.val = 342}, {.val = 358}, {.val = 376}, {.val = 401}, {.val = 430}, {.val = 462}, {.val = 498}, {.val = 498}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_8200_config[] = {
    {.val = 665}, {.val = 596}, {.val = 534}, {.val = 483}, {.val = 439}, {.val = 404}, {.val = 378}, {.val = 360}, {.val = 354}, {.val = 347}, {.val = 341}, {.val = 348}, {.val = 361}, {.val = 383}, {.val = 409}, {.val = 444}, {.val = 489}, {.val = 539}, {.val = 596}, {.val = 663}, {.val = 663}, {.val = 622}, {.val = 552}, {.val = 493}, {.val = 445}, {.val = 402}, {.val = 369}, {.val = 344}, {.val = 327}, {.val = 321}, {.val = 315}, {.val = 309}, {.val = 315}, {.val = 327}, {.val = 346}, {.val = 373}, {.val = 406}, {.val = 450}, {.val = 500}, {.val = 557}, {.val = 622}, {.val = 622}, {.val = 586}, {.val = 521}, {.val = 463}, {.val = 414}, {.val = 374}, {.val = 342}, {.val = 318}, {.val = 300}, {.val = 292}, {.val = 287}, {.val = 285}, {.val = 291}, {.val = 302}, {.val = 319}, {.val = 344}, {.val = 378}, {.val = 419}, {.val = 468}, {.val = 526}, {.val = 590}, {.val = 590}, {.val = 564}, {.val = 497}, {.val = 440}, {.val = 393}, {.val = 354}, {.val = 324}, {.val = 301}, {.val = 284}, {.val = 274}, {.val = 269}, {.val = 270}, {.val = 275}, {.val = 285}, {.val = 302}, {.val = 326}, {.val = 358}, {.val = 398}, {.val = 448}, {.val = 503}, {.val = 568}, {.val = 568}, {.val = 548}, {.val = 482}, {.val = 427}, {.val = 379}, {.val = 342}, {.val = 312}, {.val = 290}, {.val = 274}, {.val = 264}, {.val = 260}, {.val = 260}, {.val = 264}, {.val = 274}, {.val = 291}, {.val = 314}, {.val = 346}, {.val = 385}, {.val = 434}, {.val = 490}, {.val = 552}, {.val = 552}, {.val = 542}, {.val = 476}, {.val = 421}, {.val = 374}, {.val = 337}, {.val = 306}, {.val = 284}, {.val = 269}, {.val = 260}, {.val = 255}, {.val = 255}, {.val = 260}, {.val = 270}, {.val = 287}, {.val = 310}, {.val = 341}, {.val = 380}, {.val = 429}, {.val = 483}, {.val = 546}, {.val = 546}, {.val = 542}, {.val = 478}, {.val = 423}, {.val = 377}, {.val = 338}, {.val = 308}, {.val = 286}, {.val = 271}, {.val = 261}, {.val = 256}, {.val = 256}, {.val = 261}, {.val = 271}, {.val = 288}, {.val = 312}, {.val = 343}, {.val = 383}, {.val = 430}, {.val = 485}, {.val = 547}, {.val = 547}, {.val = 553}, {.val = 488}, {.val = 431}, {.val = 385}, {.val = 347}, {.val = 317}, {.val = 293}, {.val = 277}, {.val = 267}, {.val = 262}, {.val = 263}, {.val = 268}, {.val = 279}, {.val = 296}, {.val = 321}, {.val = 353}, {.val = 393}, {.val = 441}, {.val = 496}, {.val = 559}, {.val = 559}, {.val = 571}, {.val = 507}, {.val = 450}, {.val = 402}, {.val = 363}, {.val = 331}, {.val = 308}, {.val = 291}, {.val = 281}, {.val = 276}, {.val = 276}, {.val = 282}, {.val = 293}, {.val = 312}, {.val = 337}, {.val = 370}, {.val = 411}, {.val = 458}, {.val = 515}, {.val = 578}, {.val = 578}, {.val = 599}, {.val = 534}, {.val = 475}, {.val = 428}, {.val = 386}, {.val = 355}, {.val = 330}, {.val = 313}, {.val = 302}, {.val = 296}, {.val = 297}, {.val = 303}, {.val = 315}, {.val = 334}, {.val = 360}, {.val = 394}, {.val = 436}, {.val = 486}, {.val = 544}, {.val = 608}, {.val = 608}, {.val = 635}, {.val = 570}, {.val = 511}, {.val = 462}, {.val = 420}, {.val = 387}, {.val = 360}, {.val = 343}, {.val = 330}, {.val = 325}, {.val = 325}, {.val = 332}, {.val = 346}, {.val = 366}, {.val = 394}, {.val = 428}, {.val = 471}, {.val = 521}, {.val = 580}, {.val = 643}, {.val = 643}, {.val = 664}, {.val = 594}, {.val = 539}, {.val = 488}, {.val = 445}, {.val = 410}, {.val = 386}, {.val = 365}, {.val = 354}, {.val = 348}, {.val = 347}, {.val = 356}, {.val = 369}, {.val = 389}, {.val = 417}, {.val = 456}, {.val = 496}, {.val = 551}, {.val = 607}, {.val = 669}, {.val = 669}, {.val = 664}, {.val = 594}, {.val = 539}, {.val = 488}, {.val = 445}, {.val = 410}, {.val = 386}, {.val = 365}, {.val = 354}, {.val = 348}, {.val = 347}, {.val = 356}, {.val = 369}, {.val = 389}, {.val = 417}, {.val = 456}, {.val = 496}, {.val = 551}, {.val = 607}, {.val = 669}, {.val = 669}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_8200_config[] = {
    {.val = 554}, {.val = 506}, {.val = 463}, {.val = 428}, {.val = 396}, {.val = 372}, {.val = 353}, {.val = 339}, {.val = 336}, {.val = 330}, {.val = 323}, {.val = 327}, {.val = 336}, {.val = 351}, {.val = 369}, {.val = 391}, {.val = 420}, {.val = 453}, {.val = 493}, {.val = 535}, {.val = 535}, {.val = 524}, {.val = 475}, {.val = 434}, {.val = 399}, {.val = 369}, {.val = 345}, {.val = 327}, {.val = 315}, {.val = 310}, {.val = 306}, {.val = 299}, {.val = 303}, {.val = 311}, {.val = 323}, {.val = 342}, {.val = 365}, {.val = 393}, {.val = 427}, {.val = 466}, {.val = 509}, {.val = 509}, {.val = 500}, {.val = 453}, {.val = 412}, {.val = 377}, {.val = 348}, {.val = 325}, {.val = 307}, {.val = 293}, {.val = 287}, {.val = 282}, {.val = 281}, {.val = 284}, {.val = 292}, {.val = 304}, {.val = 322}, {.val = 344}, {.val = 372}, {.val = 406}, {.val = 444}, {.val = 488}, {.val = 488}, {.val = 484}, {.val = 436}, {.val = 396}, {.val = 361}, {.val = 333}, {.val = 310}, {.val = 292}, {.val = 280}, {.val = 272}, {.val = 268}, {.val = 268}, {.val = 271}, {.val = 278}, {.val = 290}, {.val = 307}, {.val = 329}, {.val = 357}, {.val = 390}, {.val = 429}, {.val = 472}, {.val = 472}, {.val = 473}, {.val = 425}, {.val = 385}, {.val = 350}, {.val = 323}, {.val = 300}, {.val = 283}, {.val = 271}, {.val = 264}, {.val = 260}, {.val = 260}, {.val = 263}, {.val = 270}, {.val = 281}, {.val = 298}, {.val = 320}, {.val = 348}, {.val = 380}, {.val = 420}, {.val = 463}, {.val = 463}, {.val = 467}, {.val = 421}, {.val = 380}, {.val = 346}, {.val = 318}, {.val = 296}, {.val = 279}, {.val = 267}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 266}, {.val = 277}, {.val = 294}, {.val = 316}, {.val = 343}, {.val = 377}, {.val = 415}, {.val = 458}, {.val = 458}, {.val = 467}, {.val = 420}, {.val = 381}, {.val = 347}, {.val = 319}, {.val = 297}, {.val = 280}, {.val = 268}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 259}, {.val = 266}, {.val = 278}, {.val = 295}, {.val = 317}, {.val = 345}, {.val = 378}, {.val = 417}, {.val = 460}, {.val = 460}, {.val = 473}, {.val = 427}, {.val = 387}, {.val = 353}, {.val = 325}, {.val = 302}, {.val = 285}, {.val = 272}, {.val = 265}, {.val = 261}, {.val = 261}, {.val = 264}, {.val = 272}, {.val = 284}, {.val = 301}, {.val = 323}, {.val = 352}, {.val = 385}, {.val = 423}, {.val = 467}, {.val = 467}, {.val = 485}, {.val = 439}, {.val = 398}, {.val = 365}, {.val = 336}, {.val = 313}, {.val = 295}, {.val = 283}, {.val = 274}, {.val = 270}, {.val = 270}, {.val = 274}, {.val = 282}, {.val = 295}, {.val = 313}, {.val = 336}, {.val = 364}, {.val = 397}, {.val = 436}, {.val = 480}, {.val = 480}, {.val = 502}, {.val = 456}, {.val = 416}, {.val = 381}, {.val = 353}, {.val = 329}, {.val = 311}, {.val = 298}, {.val = 289}, {.val = 285}, {.val = 285}, {.val = 290}, {.val = 298}, {.val = 312}, {.val = 330}, {.val = 353}, {.val = 381}, {.val = 414}, {.val = 454}, {.val = 497}, {.val = 497}, {.val = 525}, {.val = 479}, {.val = 439}, {.val = 404}, {.val = 375}, {.val = 351}, {.val = 332}, {.val = 319}, {.val = 310}, {.val = 306}, {.val = 305}, {.val = 310}, {.val = 319}, {.val = 333}, {.val = 352}, {.val = 375}, {.val = 404}, {.val = 438}, {.val = 477}, {.val = 520}, {.val = 520}, {.val = 544}, {.val = 498}, {.val = 457}, {.val = 421}, {.val = 392}, {.val = 367}, {.val = 348}, {.val = 334}, {.val = 326}, {.val = 322}, {.val = 321}, {.val = 327}, {.val = 336}, {.val = 349}, {.val = 369}, {.val = 393}, {.val = 421}, {.val = 454}, {.val = 493}, {.val = 537}, {.val = 537}, {.val = 544}, {.val = 498}, {.val = 457}, {.val = 421}, {.val = 392}, {.val = 367}, {.val = 348}, {.val = 334}, {.val = 326}, {.val = 322}, {.val = 321}, {.val = 327}, {.val = 336}, {.val = 349}, {.val = 369}, {.val = 393}, {.val = 421}, {.val = 454}, {.val = 493}, {.val = 537}, {.val = 537}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_8200_config[] = {
    {.val = 537}, {.val = 492}, {.val = 452}, {.val = 418}, {.val = 389}, {.val = 365}, {.val = 347}, {.val = 334}, {.val = 332}, {.val = 326}, {.val = 319}, {.val = 323}, {.val = 332}, {.val = 347}, {.val = 364}, {.val = 386}, {.val = 414}, {.val = 446}, {.val = 483}, {.val = 524}, {.val = 524}, {.val = 512}, {.val = 467}, {.val = 426}, {.val = 393}, {.val = 364}, {.val = 341}, {.val = 323}, {.val = 311}, {.val = 307}, {.val = 302}, {.val = 296}, {.val = 300}, {.val = 308}, {.val = 320}, {.val = 338}, {.val = 361}, {.val = 388}, {.val = 420}, {.val = 457}, {.val = 499}, {.val = 499}, {.val = 493}, {.val = 447}, {.val = 407}, {.val = 374}, {.val = 345}, {.val = 322}, {.val = 304}, {.val = 291}, {.val = 284}, {.val = 280}, {.val = 278}, {.val = 282}, {.val = 290}, {.val = 302}, {.val = 319}, {.val = 341}, {.val = 368}, {.val = 401}, {.val = 436}, {.val = 479}, {.val = 479}, {.val = 479}, {.val = 434}, {.val = 393}, {.val = 360}, {.val = 331}, {.val = 308}, {.val = 291}, {.val = 279}, {.val = 271}, {.val = 267}, {.val = 266}, {.val = 270}, {.val = 277}, {.val = 288}, {.val = 305}, {.val = 327}, {.val = 354}, {.val = 386}, {.val = 423}, {.val = 465}, {.val = 465}, {.val = 471}, {.val = 425}, {.val = 385}, {.val = 351}, {.val = 322}, {.val = 300}, {.val = 284}, {.val = 271}, {.val = 263}, {.val = 260}, {.val = 259}, {.val = 262}, {.val = 269}, {.val = 280}, {.val = 297}, {.val = 318}, {.val = 345}, {.val = 377}, {.val = 414}, {.val = 456}, {.val = 456}, {.val = 469}, {.val = 422}, {.val = 382}, {.val = 348}, {.val = 320}, {.val = 297}, {.val = 280}, {.val = 268}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 258}, {.val = 265}, {.val = 277}, {.val = 293}, {.val = 315}, {.val = 342}, {.val = 374}, {.val = 410}, {.val = 453}, {.val = 453}, {.val = 472}, {.val = 425}, {.val = 384}, {.val = 351}, {.val = 322}, {.val = 299}, {.val = 282}, {.val = 269}, {.val = 261}, {.val = 257}, {.val = 256}, {.val = 259}, {.val = 266}, {.val = 278}, {.val = 294}, {.val = 316}, {.val = 344}, {.val = 376}, {.val = 413}, {.val = 455}, {.val = 455}, {.val = 481}, {.val = 434}, {.val = 393}, {.val = 359}, {.val = 329}, {.val = 306}, {.val = 288}, {.val = 275}, {.val = 267}, {.val = 262}, {.val = 262}, {.val = 265}, {.val = 272}, {.val = 284}, {.val = 301}, {.val = 324}, {.val = 351}, {.val = 383}, {.val = 421}, {.val = 463}, {.val = 463}, {.val = 495}, {.val = 449}, {.val = 407}, {.val = 372}, {.val = 342}, {.val = 318}, {.val = 300}, {.val = 286}, {.val = 277}, {.val = 273}, {.val = 272}, {.val = 275}, {.val = 283}, {.val = 296}, {.val = 314}, {.val = 337}, {.val = 365}, {.val = 397}, {.val = 434}, {.val = 476}, {.val = 476}, {.val = 516}, {.val = 469}, {.val = 428}, {.val = 391}, {.val = 361}, {.val = 336}, {.val = 317}, {.val = 303}, {.val = 293}, {.val = 289}, {.val = 288}, {.val = 291}, {.val = 300}, {.val = 314}, {.val = 331}, {.val = 355}, {.val = 383}, {.val = 416}, {.val = 453}, {.val = 495}, {.val = 495}, {.val = 543}, {.val = 495}, {.val = 453}, {.val = 416}, {.val = 386}, {.val = 361}, {.val = 340}, {.val = 326}, {.val = 316}, {.val = 311}, {.val = 310}, {.val = 314}, {.val = 323}, {.val = 336}, {.val = 355}, {.val = 378}, {.val = 407}, {.val = 439}, {.val = 477}, {.val = 519}, {.val = 519}, {.val = 562}, {.val = 515}, {.val = 472}, {.val = 434}, {.val = 405}, {.val = 379}, {.val = 358}, {.val = 343}, {.val = 333}, {.val = 328}, {.val = 327}, {.val = 331}, {.val = 340}, {.val = 354}, {.val = 373}, {.val = 396}, {.val = 425}, {.val = 458}, {.val = 495}, {.val = 537}, {.val = 537}, {.val = 562}, {.val = 515}, {.val = 472}, {.val = 434}, {.val = 405}, {.val = 379}, {.val = 358}, {.val = 343}, {.val = 333}, {.val = 328}, {.val = 327}, {.val = 331}, {.val = 340}, {.val = 354}, {.val = 373}, {.val = 396}, {.val = 425}, {.val = 458}, {.val = 495}, {.val = 537}, {.val = 537}
};

static const isp_lsc_gain_t s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_8200_config[] = {
    {.val = 506}, {.val = 464}, {.val = 429}, {.val = 399}, {.val = 373}, {.val = 353}, {.val = 337}, {.val = 326}, {.val = 325}, {.val = 319}, {.val = 312}, {.val = 315}, {.val = 322}, {.val = 334}, {.val = 349}, {.val = 365}, {.val = 389}, {.val = 417}, {.val = 446}, {.val = 483}, {.val = 483}, {.val = 484}, {.val = 443}, {.val = 407}, {.val = 376}, {.val = 351}, {.val = 331}, {.val = 316}, {.val = 306}, {.val = 301}, {.val = 299}, {.val = 292}, {.val = 294}, {.val = 301}, {.val = 311}, {.val = 326}, {.val = 344}, {.val = 368}, {.val = 394}, {.val = 426}, {.val = 462}, {.val = 462}, {.val = 466}, {.val = 426}, {.val = 390}, {.val = 360}, {.val = 335}, {.val = 314}, {.val = 299}, {.val = 287}, {.val = 282}, {.val = 278}, {.val = 276}, {.val = 279}, {.val = 284}, {.val = 294}, {.val = 308}, {.val = 328}, {.val = 351}, {.val = 379}, {.val = 409}, {.val = 445}, {.val = 445}, {.val = 457}, {.val = 414}, {.val = 378}, {.val = 348}, {.val = 323}, {.val = 302}, {.val = 287}, {.val = 276}, {.val = 269}, {.val = 267}, {.val = 266}, {.val = 268}, {.val = 274}, {.val = 283}, {.val = 297}, {.val = 315}, {.val = 339}, {.val = 366}, {.val = 398}, {.val = 434}, {.val = 434}, {.val = 449}, {.val = 406}, {.val = 372}, {.val = 341}, {.val = 315}, {.val = 295}, {.val = 280}, {.val = 269}, {.val = 263}, {.val = 260}, {.val = 259}, {.val = 261}, {.val = 267}, {.val = 276}, {.val = 290}, {.val = 309}, {.val = 332}, {.val = 360}, {.val = 391}, {.val = 426}, {.val = 426}, {.val = 448}, {.val = 406}, {.val = 369}, {.val = 339}, {.val = 312}, {.val = 292}, {.val = 277}, {.val = 266}, {.val = 259}, {.val = 256}, {.val = 256}, {.val = 258}, {.val = 263}, {.val = 273}, {.val = 287}, {.val = 305}, {.val = 329}, {.val = 357}, {.val = 388}, {.val = 424}, {.val = 424}, {.val = 450}, {.val = 408}, {.val = 372}, {.val = 340}, {.val = 314}, {.val = 294}, {.val = 278}, {.val = 267}, {.val = 260}, {.val = 256}, {.val = 256}, {.val = 258}, {.val = 264}, {.val = 274}, {.val = 288}, {.val = 307}, {.val = 330}, {.val = 358}, {.val = 390}, {.val = 426}, {.val = 426}, {.val = 458}, {.val = 415}, {.val = 378}, {.val = 346}, {.val = 321}, {.val = 299}, {.val = 284}, {.val = 272}, {.val = 265}, {.val = 261}, {.val = 261}, {.val = 263}, {.val = 269}, {.val = 279}, {.val = 294}, {.val = 313}, {.val = 337}, {.val = 365}, {.val = 397}, {.val = 434}, {.val = 434}, {.val = 471}, {.val = 428}, {.val = 390}, {.val = 359}, {.val = 332}, {.val = 310}, {.val = 294}, {.val = 282}, {.val = 274}, {.val = 270}, {.val = 270}, {.val = 273}, {.val = 279}, {.val = 290}, {.val = 305}, {.val = 324}, {.val = 348}, {.val = 376}, {.val = 408}, {.val = 444}, {.val = 444}, {.val = 490}, {.val = 446}, {.val = 407}, {.val = 375}, {.val = 348}, {.val = 326}, {.val = 309}, {.val = 296}, {.val = 288}, {.val = 284}, {.val = 283}, {.val = 287}, {.val = 294}, {.val = 305}, {.val = 320}, {.val = 340}, {.val = 364}, {.val = 392}, {.val = 424}, {.val = 461}, {.val = 461}, {.val = 513}, {.val = 470}, {.val = 430}, {.val = 397}, {.val = 369}, {.val = 347}, {.val = 329}, {.val = 316}, {.val = 307}, {.val = 304}, {.val = 303}, {.val = 306}, {.val = 313}, {.val = 326}, {.val = 341}, {.val = 362}, {.val = 385}, {.val = 413}, {.val = 445}, {.val = 480}, {.val = 480}, {.val = 532}, {.val = 487}, {.val = 448}, {.val = 413}, {.val = 386}, {.val = 363}, {.val = 345}, {.val = 332}, {.val = 324}, {.val = 318}, {.val = 318}, {.val = 322}, {.val = 328}, {.val = 342}, {.val = 357}, {.val = 376}, {.val = 399}, {.val = 428}, {.val = 461}, {.val = 494}, {.val = 494}, {.val = 532}, {.val = 487}, {.val = 448}, {.val = 413}, {.val = 386}, {.val = 363}, {.val = 345}, {.val = 332}, {.val = 324}, {.val = 318}, {.val = 318}, {.val = 322}, {.val = 328}, {.val = 342}, {.val = 357}, {.val = 376}, {.val = 399}, {.val = 428}, {.val = 461}, {.val = 494}, {.val = 494}
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
    {
        .color_temp = 4637,
        .lsc = {
            .gain_r  = s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_4637_config,
            .gain_gr = s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_4637_config,
            .gain_gb = s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_4637_config,
            .gain_b  = s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_4637_config,
            .lsc_gain_array_size = 273
        },
    },
    {
        .color_temp = 5210,
        .lsc = {
            .gain_r  = s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_5210_config,
            .gain_gr = s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_5210_config,
            .gain_gb = s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_5210_config,
            .gain_b  = s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_5210_config,
            .lsc_gain_array_size = 273
        },
    },
    {
        .color_temp = 5925,
        .lsc = {
            .gain_r  = s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_5925_config,
            .gain_gr = s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_5925_config,
            .gain_gb = s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_5925_config,
            .gain_b  = s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_5925_config,
            .lsc_gain_array_size = 273
        },
    },
    {
        .color_temp = 6254,
        .lsc = {
            .gain_r  = s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_6254_config,
            .gain_gr = s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_6254_config,
            .gain_gb = s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_6254_config,
            .gain_b  = s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_6254_config,
            .lsc_gain_array_size = 273
        },
    },
    {
        .color_temp = 6746,
        .lsc = {
            .gain_r  = s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_6746_config,
            .gain_gr = s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_6746_config,
            .gain_gb = s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_6746_config,
            .gain_b  = s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_6746_config,
            .lsc_gain_array_size = 273
        },
    },
    {
        .color_temp = 7378,
        .lsc = {
            .gain_r  = s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_7378_config,
            .gain_gr = s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_7378_config,
            .gain_gb = s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_7378_config,
            .gain_b  = s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_7378_config,
            .lsc_gain_array_size = 273
        },
    },
    {
        .color_temp = 8200,
        .lsc = {
            .gain_r  = s_esp_ipa_acc_lsc_gain_r_SC202CS_1280_x_720_ct_8200_config,
            .gain_gr = s_esp_ipa_acc_lsc_gain_gr_SC202CS_1280_x_720_ct_8200_config,
            .gain_gb = s_esp_ipa_acc_lsc_gain_gb_SC202CS_1280_x_720_ct_8200_config,
            .gain_b  = s_esp_ipa_acc_lsc_gain_b_SC202CS_1280_x_720_ct_8200_config,
            .lsc_gain_array_size = 273
        },
    },
};

static const esp_ipa_acc_lsc_t s_esp_ipa_acc_lsc_SC202CS_config[] = {
    {
        .width = 1280,
        .height = 720,
        .lsc_gain_table = s_esp_ipa_acc_lsc_SC202CS_1280_x_720_config,
        .lsc_gain_table_size = 9
    }
};

static const esp_ipa_acc_blc_param_t s_ipa_acc_blc_SC202CS_table[] = {
    {
        .gain = 1,
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
    .lsc_disable_gain = 0,
    .blc = &s_ipa_acc_blc_SC202CS_config,
};

static const esp_ipa_aen_gamma_unit_t s_esp_ipa_aen_gamma_SC202CS_table[] = {
    {
        .luma = 15.1,
        .gamma = {
            .red = {
                .x = { 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255,  },
                .y = { 66, 93, 114, 132, 147, 161, 174, 186, 197, 208, 218, 228, 237, 246, 255, 255,  }
            },
            .green = {
                .x = { 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255,  },
                .y = { 66, 93, 114, 132, 147, 161, 174, 186, 197, 208, 218, 228, 237, 246, 255, 255,  }
            },
            .blue = {
                .x = { 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255,  },
                .y = { 66, 93, 114, 132, 147, 161, 174, 186, 197, 208, 218, 228, 237, 246, 255, 255,  }
            }
        }
    },
    {
        .luma = 30.1,
        .gamma = {
            .red = {
                .x = { 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255,  },
                .y = { 56, 82, 104, 122, 138, 153, 166, 179, 192, 203, 214, 225, 235, 245, 255, 255,  }
            },
            .green = {
                .x = { 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255,  },
                .y = { 56, 82, 104, 122, 138, 153, 166, 179, 192, 203, 214, 225, 235, 245, 255, 255,  }
            },
            .blue = {
                .x = { 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255,  },
                .y = { 56, 82, 104, 122, 138, 153, 166, 179, 192, 203, 214, 225, 235, 245, 255, 255,  }
            }
        }
    },
    {
        .luma = 90.1,
        .gamma = {
            .red = {
                .x = { 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255,  },
                .y = { 50, 75, 96, 115, 131, 146, 161, 174, 187, 199, 211, 223, 234, 245, 255, 255,  }
            },
            .green = {
                .x = { 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255,  },
                .y = { 50, 75, 96, 115, 131, 146, 161, 174, 187, 199, 211, 223, 234, 245, 255, 255,  }
            },
            .blue = {
                .x = { 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255,  },
                .y = { 50, 75, 96, 115, 131, 146, 161, 174, 187, 199, 211, 223, 234, 245, 255, 255,  }
            }
        }
    },
    {
        .luma = 300.1,
        .gamma = {
            .red = {
                .x = { 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255,  },
                .y = { 43, 68, 89, 107, 124, 140, 155, 169, 182, 195, 208, 220, 232, 244, 255, 255,  }
            },
            .green = {
                .x = { 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255,  },
                .y = { 43, 68, 89, 107, 124, 140, 155, 169, 182, 195, 208, 220, 232, 244, 255, 255,  }
            },
            .blue = {
                .x = { 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255,  },
                .y = { 43, 68, 89, 107, 124, 140, 155, 169, 182, 195, 208, 220, 232, 244, 255, 255,  }
            }
        }
    },
};

static const esp_ipa_aen_gamma_config_t s_ipa_aen_gamma_SC202CS_config = {
    .model = 0,
    .luma_env = "env.luma.avg",
    .luma_min_step = 3.0,
    .gamma_table = s_esp_ipa_aen_gamma_SC202CS_table,
    .gamma_table_size = 4,
};

static const esp_ipa_aen_sharpen_t s_ipa_aen_sharpen_SC202CS_config[] = {
    {
        .gain = 1000,
        .sharpen = {
            .h_thresh = 16,
            .l_thresh = 5,
            .h_coeff = 1.625,
            .m_coeff = 1.525,
            .matrix = {
                {1, 2, 1},
                {2, 1, 2},
                {1, 2, 1}
            }
        }
    },
    {
        .gain = 8000,
        .sharpen = {
            .h_thresh = 20,
            .l_thresh = 5,
            .h_coeff = 1.625,
            .m_coeff = 1.425,
            .matrix = {
                {2, 2, 2},
                {2, 1, 2},
                {2, 2, 2}
            }
        }
    },
    {
        .gain = 12000,
        .sharpen = {
            .h_thresh = 16,
            .l_thresh = 5,
            .h_coeff = 1.625,
            .m_coeff = 1.325,
            .matrix = {
                {1, 1, 1},
                {1, 1, 1},
                {1, 1, 1}
            }
        }
    },
    {
        .gain = 65000,
        .sharpen = {
            .h_thresh = 20,
            .l_thresh = 5,
            .h_coeff = 1.625,
            .m_coeff = 1.225,
            .matrix = {
                {1, 1, 1},
                {1, 1, 1},
                {1, 1, 1}
            }
        }
    },
};

static const esp_ipa_aen_con_t s_ipa_aen_con_SC202CS_config[] = {
    {
        .gain = 1000,
        .contrast = 132
    },
    {
        .gain = 16000,
        .contrast = 130
    },
    {
        .gain = 24000,
        .contrast = 128
    },
    {
        .gain = 65000,
        .contrast = 126
    },
};

static const esp_ipa_aen_config_t s_ipa_aen_SC202CS_config = {
    .gamma = &s_ipa_aen_gamma_SC202CS_config,
    .sharpen_table = s_ipa_aen_sharpen_SC202CS_config,
    .sharpen_table_size = ARRAY_SIZE(s_ipa_aen_sharpen_SC202CS_config),
    .con_table = s_ipa_aen_con_SC202CS_config,
    .con_table_size = ARRAY_SIZE(s_ipa_aen_con_SC202CS_config),
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

/* Json file: components/m5_tab5_camera/sc202cs_default.json */

