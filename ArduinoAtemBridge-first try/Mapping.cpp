#include "Mapping.h"

Mapping::Mapping() noexcept {
    buttons_odd = std::unordered_map<int, std::string> {
        /**
         * ME
        */
        // 0. row: above ME
        

        // 1 row: key busses
        {57, "ME.1.1"},
        {49, "ME.1.2"},
        {41, "ME.1.3"},
        {33, "ME.1.4"},
        {25, "ME.1.5"},
        {17, "ME.1.6"},
        // 2. row: background B
        {121, "ME.2.1"},
        {113, "ME.2.2"},
        {105, "ME.2.3"},
        {97, "ME.2.4"},
        {89, "ME.2.5"},
        {81, "ME.2.6"},
        // 3. row: background a
        {185, "ME.3.1"},
        {177, "ME.3.2"},
        {169, "ME.3.3"},
        {161, "ME.3.4"},
        {153, "ME.3.5"},
        {145, "ME.3.6"},

        /**
            * PP
        */
        // 1. row: key busses
        {56, "PP.1.1"},
        {48, "PP.1.2"},
        {40, "PP.1.3"},
        {32, "PP.1.4"},
        {24, "PP.1.5"},
        {16, "PP.1.6"},
        // 2. row: programm
        {120, "PP.2.1"},
        {112, "PP.2.2"},
        {104, "PP.2.3"},
        {96, "PP.2.4"},
        {88, "PP.2.5"},
        {80, "PP.2.6"},
        // 3. row: preset
        {184, "PP.3.1"},
        {176, "PP.3.2"},
        {168, "PP.3.3"},
        {160, "PP.3.4"},
        {152, "PP.3.5"},
        {144, "PP.3.6"},

        /**
            * Wipe
        */
        // fat block
        {39, "Wipe.A"},
        {103, "Wipe.B"},
        {167, "Wipe.C"},
        {38, "Wipe.D"},
        {102, "Wipe.E"},
        {47, "Wipe.F"},
        {111, "Wipe.G"},
        {175, "Wipe.H"},
        {46, "Wipe.I"},
        {110, "Wipe.J"},
        {55, "Wipe.K"},
        {119, "Wipe.L"},
        {183, "Wipe.M"},
        {54, "Wipe.N"},
        {118, "Wipe.O"},
        {63, "Wipe.P"},
        {127, "Wipe.Q"},
        {191, "Wipe.R"},
        {62, "Wipe.S"},
        {126, "Wipe.T"},
        {15, "Wipe.U"},
        {79, "Wipe.V"},
        {143, "Wipe.W"},
        {14, "Wipe.X"},
        {78, "Wipe.Y"},
        {23, "Wipe.Z"},
        {87, "Wipe.+"},
        {151, "Wipe.-"},
        {22, "Wipe.*"},
        {86, "Wipe./"},
        {31, "Wipe.U1"},
        {95, "Wipe.U2"},
        {159, "Wipe.U3"},
        {30, "Wipe.U4"},
        {94, "Wipe.U5"},
        // around block
        {174, "Wipe.ext_pos"},
        {166, "Wipe.pos"},
        {37, "Wipe.rot_user"},
        {182, "Wipe.text"},
        {45, "Wipe.flipflop"},
        {53, "Wipe.rev"},
        {70, "Wipe.ME2"},
        {6, "Wipe.ME1"},
        // above poti
        {190, "Wipe.1"},
        {158, "Wipe.2"},
        {150, "Wipe.3"},
        {142, "Wipe.4"},
        // below poti
        {61, "Wipe.mod"},
        {29, "Wipe.multi"},
        {21, "Wipe.wipe_comb"},
        {13, "Wipe.wipe_rot"},
        {109, "Wipe.board"},
        {101, "Wipe.soft"},
        {85, "Wipe.aspect"},
        {77, "Wipe.matrix"},
        // colored shit and above
        {93, "Wipe.mod_reset"},
        {125, "Wipe.DVE1"},
        {117, "Wipe.DVE2"},
        {181, "Wipe.ME1_w1"},
        {173, "Wipe.ME1_w2"},
        {189, "Wipe.ME2_w1"},
        {165, "Wipe.ME2_w2"},
        {157, "Wipe.DSK_w1"},
        {149, "Wipe.DSK_w2"},

        /**
            * to dve
        */
        {250, "tDVE.loop"},
        {242, "tDVE.preset"},
        {234, "tDVE.pgm"},
        {226, "tDVE.key1"},
        {218, "tDVE.key2"},
        {210, "tDVE.me1"},
        {202, "tDVE.me2"},
        {314, "tDVE.pp"},

        /**
            * delegation
        */
        {306, "deleg.dve1"},
        {298, "deleg.dve2"},
        {290, "deleg.aux1"},
        {282, "deleg.aux2"},
        {274, "deleg.aux3"},
        {266, "deleg.aux4"},
        {258, "deleg.aux5"},

        /**
            * transition ME
        */
        {245, "tME.limit_set"},
        {237, "tME.limit_on"},
        {229, "tME.bgdl"},
        {309, "tME.bgdr"},
        {252, "tME.key1"},
        {244, "tME.key2"},
        {221, "tME.add"},
        {213, "tME.dve"},
        {301, "tME.mix"},
        {293, "tME.wipe1"},
        {285, "tME.wipe2"},
        {205, "tME.blk_pst"},
        {277, "tME.trans_dur"},
        {269, "tME.trans_pvw"},
        {253, "tME.auto"},
        {317, "tME.cut"},
        /**
            * transition PP
        */
        {251, "tPP.limit_set"},
        {243, "tPP.limit_on"},
        {332, "tPP.bgdl"},
        {316, "tPP.bgdr"},
        {308, "tPP.dsk1"},
        {300, "tPP.dsk2"},
        {292, "tPP.add"},
        {284, "tPP.dve"},
        {276, "tPP.mix"},
        {268, "tPP.wipe1"},
        {364, "tPP.wipe2"},
        {356, "tPP.blk_pst"},
        {348, "tPP.trans_dur"},
        {340, "tPP.trans_pvw"},
        {380, "tPP.auto"},
        {372, "tPP.cut"},

        /**
            * Keyers
        */
        {236, "key.shdw_store"},
        {228, "key.bord"},
        {220, "key.shdw"},
        {212, "key.drop"},
        {204, "key.outline"},
        {254, "key.key_over"},
        {246, "key.key_inv"},
        {238, "key.mask_on"},
        {230, "key.mask_key"},
        {222, "key.key_pvw"},
        {318, "key.matte"},
        {310, "key.key_bus_l"},
        {302, "key.key_bus_r"},
        {294, "key.cupl_split"},
        {382, "key.add_lin"},
        {374, "key.lum_lin"},
        {286, "key.chr_key"},
        {278, "key.pattn1"},
        {270, "key.pattn2"},
        {366, "key.cut"},
        {358, "key.bgda"},
        {350, "key.bgdb"},
        {342, "key.key1"},
        {334, "key.key2"},
        {214, "key.auto"},

        /**
            * DSK
        */
        {255, "dsk.coupl"},
        {235, "dsk.trans_dur_l"},
        {227, "dsk.trans_dur_r"},
        {315, "dsk.cut_l"},
        {307, "dsk.cut_r"},
        {299, "dsk.auto_l"},
        {291, "dsk.auto_r"},
        {247, "dsk.shdw_store"},
        {239, "dsk.bord"},
        {231, "dsk.shdw"},
        {223, "dsk.drop"},
        {215, "dsk.outline"},
        {319, "dsk.key_over"},
        {311, "dsk.key_inv"},
        {303, "dsk.mask_on"},
        {295, "dsk.mask_key"},
        {287, "dsk.key_pvw"},
        {279, "dsk.matte"},
        {271, "dsk.key_bus_l"},
        {383, "dsk.key_bus_r"},
        {375, "dsk.cupl_split"},
        {283, "dsk.add_lin"},
        {275, "dsk.lum_lin"},
        {267, "dsk.chr_key"},
        {367, "dsk.pattn1"},
        {359, "dsk.pattn2"},
        {327, "dsk.bgda"},
        {219, "dsk.bgdb"},
        {351, "dsk.dsk1"},
        {343, "dsk.dsk2"},
        {335, "dsk.auto"},
        
        /**
            * Chroma Keyers
        */

        /**
            * Positioner
        */
        
        /**
            * Extra
        */
        
        /**
         * ftb
        */
        
        /**
            * Display
        */
        
    };

    buttons_even = std::unordered_map<int, std::string>{
        /**
         * ME
        */
        // 0. row: above ME
        {61, "desti.7"},
        //{53, "desti.8"},
        {45, "desti.9"},
        {37, "desti.10"},
        {29, "desti.11"},
        {21, "desti.12"},
        {13, "desti.13"},
        {5, "desti.14"},
        {125, "desti.15"},
        {117, "desti.16"},
        {109, "desti.17"},
        {101, "desti.18"},
        {93, "desti.19"},
        {85, "desti.20"},
        {77, "desti.21"},
        {69, "desti.22"},
        
        // special keys
        {36, "ME.extra"},
        {28, "ME.2nd"},
        // 1. row: key busses
        {56, "ME.1.7"},
        {48,"ME.1.8"},
        {40,"ME.1.9"},
        {32,"ME.1.10"},
        {24,"ME.1.11"},
        {16,"ME.1.12"},
        {8,"ME.1.13"},
        {58,"ME.1.14"},
        {50,"ME.1.15"},
        {42,"ME.1.16"},
        {34,"ME.1.17"},
        {26,"ME.1.18"},
        {18,"ME.1.19"},
        {10,"ME.1.20"},
        {44,"ME.1.21"},
        // 2. row: background B
        {120, "ME.2.7"},
        {112,"ME.2.8"},
        {104,"ME.2.9"},
        {96,"ME.2.10"},
        {88,"ME.2.11"},
        {80,"ME.2.12"},
        {72,"ME.2.13"},
        {122,"ME.2.14"},
        {114,"ME.2.15"},
        {106,"ME.2.16"},
        {98,"ME.2.17"},
        {90,"ME.2.18"},
        {82,"ME.2.19"},
        {74,"ME.2.20"},
        {52,"ME.2.21"},
        // 3. row: background a
        {184, "ME.3.7"},
        {176,"ME.3.8"},
        {168,"ME.3.9"},
        {160,"ME.3.10"},
        {152,"ME.3.11"},
        {144,"ME.3.12"},
        {136,"ME.3.13"},
        {186,"ME.3.14"},
        {178,"ME.3.15"},
        {170,"ME.3.16"},
        {162,"ME.3.17"},
        {154,"ME.3.18"},
        {146,"ME.3.19"},
        {138,"ME.3.20"},
        {60,"ME.3.21"},

        /**
            * PP
        */
        // special keys
        {92, "PP.2nd"},
        // 1. row: key busses
        {57, "PP.1.7"},
        {49,"PP.1.8"},
        {41,"PP.1.9"},
        {33,"PP.1.10"},
        {25,"PP.1.11"},
        {17,"PP.1.12"},
        {9,"PP.1.13"},
        {59,"PP.1.14"},
        {51,"PP.1.15"},
        {43,"PP.1.16"},
        {53,"PP.1.17"},
        {27,"PP.1.18"},
        {19,"PP.1.19"},
        {11,"PP.1.20"},
        {108,"PP.1.21"},
        // 2. row: programm
        {121, "PP.2.7"},
        {113,"PP.2.8"},
        {105,"PP.2.9"},
        {97,"PP.2.10"},
        {89,"PP.2.11"},
        {81,"PP.2.12"},
        {73,"PP.2.13"},
        {123,"PP.2.14"},
        {115,"PP.2.15"},
        {107,"PP.2.16"},
        {99,"PP.2.17"},
        {91,"PP.2.18"},
        {83,"PP.2.19"},
        {75,"PP.2.20"},
        {116,"PP.2.21"},
        //    // 3. row: preset
        {185, "PP.3.7"},
        {177,"PP.3.8"},
        {169,"PP.3.9"},
        {161,"PP.3.10"},
        {153,"PP.3.11"},
        {145,"PP.3.12"},
        {137,"PP.3.13"},
        {187,"PP.3.14"},
        {179,"PP.3.15"},
        {171,"PP.3.16"},
        {163,"PP.3.17"},
        {155,"PP.3.18"},
        {147,"PP.3.19"},
        {139,"PP.3.20"},
        {124,"PP.3.21"},

        /**

            * Chroma Keyers
        */
        {476 ,"chr.key_color"},
        {468 ,"chr.cut"},
        {396 ,"chr.auto"},
        {484 ,"chr.fgd"},
        {404 ,"chr.me1_key1"},
        {436 ,"chr.me1_key2"},
        {412 ,"chr.me2_key1"},
        {444 ,"chr.me2_key2"},
        {420 ,"chr.dsk1"},
        {428 ,"chr.dsk2"},
        
        /**
            * Positioner
        */
        {492 ,"pos.h_lock"},
        {500 ,"pos.cent"},
        {508 ,"pos.v_lock"},
        
        /**
            * Extra
        */
        {310 ,"extra.above.pvw_me1"},
        {302 ,"extra.above.pvw_me2"},
        {294 ,"extra.above.pvw_pp"},
        {286 ,"extra.above.clean"},
        {228 ,"extra.flip_flop"},
        {236 ,"extra.rev"},
        {204 ,"extra.stop_next"},
        {244 ,"extra.stop"},
        {364 ,"extra.me1"},
        {356 ,"extra.col_bgd"},
        {380 ,"extra.store"},
        {325 ,"extra.bank"},
        {349 ,"extra.edit"},
        {372 ,"extra.me2"},
        {332 ,"extra.misc"},
        {221 ,"extra.pp"},
        {253 ,"extra.reloc"},
        {381 ,"extra.7"},
        {341 ,"extra.8"},
        {373 ,"extra.9"},
        {333 ,"extra.4"},
        {365 ,"extra.5"},
        {357 ,"extra.6"},
        {301 ,"extra.1"},
        {269 ,"extra.2"},
        {309 ,"extra.3"},
        {277 ,"extra.clear"},
        {317 ,"extra.0"},
        {285 ,"extra.enter"},
        {213 ,"extra.undo"},
        {205 ,"extra.cut"},
        {229 ,"extra.auto"},
        {293 ,"extra.trans_dur"},
            
        /**
         * ftb
        */
        {324 ,"ftb.ftb"},
        {340 ,"ftb.trans_dur"},
        {245 ,"ftb.cut"},
        {237 ,"ftb.auto"},

       /**
         * delegation
       */
        { 254,"deleg.aux6" },
        { 246,"deleg.video_store" },
        { 238,"deleg.mont_prog" },
        { 230,"deleg.pvw" },
        { 318,"deleg.ext_aux" },


        /**
        * Mattes
        */
        {300,"mattes.col1"},
        {292,"mattes.col2"},
        {212 ,"mattes.matte_sel" },
        {268 ,"mattes.matte1" },
        {308 ,"mattes.matte2" },
        {276 ,"mattes.flat" },
        {316 ,"mattes.wash" },
        {284 ,"mattes.spec_wash" },
        {252 ,"mattes.me1" },
        {220 ,"mattes.me2" },
        {348 ,"mattes.dsk" },


        /**
         * Display
        */
        {352 ,"display.vtr_enable"},
        {248 ,"display.edit_enable"},
        {312 ,"display.gpi_enable"},
        {376 ,"display.me1"},
        {368 ,"display.me2"},
        {360 ,"display.pp"},
        {232 ,"display.status"},
        {296 ,"display.transf"},
        {224 ,"display.extra"},
        {288 ,"display.gpi"},
        {216 ,"display.access"},
        {280 ,"display.key"},
        {208 ,"display.mont_prog"},
        {272 ,"display.install"},
        {200,"display.stores"},
        {264 ,"display.masks"},
        {289 ,"display.exit"},
        {281 ,"display.second"},
        {249 ,"display.above.1"},
        {241 ,"display.above.2"},
        {233 ,"display.above.3"},
        {225 ,"display.above.4"},
        {217 ,"display.above.5"},
        {209 ,"display.above.6"},
        {201 ,"display.above.7"},
        {313 ,"display.above.8"},
        {305 ,"display.above.9"},
        {297 ,"display.above.10"},
        {344 ,"display.below.1"},
        {336 ,"display.below.2"},
        {328 ,"display.below.3"},
        {377 ,"display.below.4"},
        {369 ,"display.below.5"},
        {361,"display.below.6"},
        {353 ,"display.below.7"},
        {345 ,"display.below.8"},
        {337 ,"display.below.9"},
        {329 ,"display.below.10"},
    };

    leds_odd = std::unordered_map<std::string, int>{
        // number of the bit as int counted from the front
        /**
         * ME
        */
        {"ME.3.4", 82}, // array 10, bit 2

        /**
         * PP
        */
        {"PP.2.6", 89}, // (11,1)
        // 3. row: preset
        {"PP.3.1", 57}, // array 7, bit 1 => 8*7+1
        {"PP.3.4", 81} // array 10, bit 1 => 8*10+1
    };


    leds_even = std::unordered_map<std::string, int> {
            /**
             * ME
            */
            // special keys
            {"ME.extra", 0},
            {"ME.2nd", 0},
            // 1. row: key busses
            {"ME.1.1", 0},
            {"ME.1.2", 0},
            {"ME.1.3", 0},
            {"ME.1.4", 0},
            {"ME.1.5", 0},
            {"ME.1.6", 0},
            {"ME.1.7", 0},
            {"ME.1.8", 0},
            {"ME.1.9", 0},
            {"ME.1.10", 0},
            {"ME.1.11", 0},
            {"ME.1.12", 0},
            {"ME.1.13", 0},
            {"ME.1.14", 0},
            {"ME.1.15", 0},
            {"ME.1.16", 0},
            {"ME.1.17", 0},
            {"ME.1.18", 0},
            {"ME.1.19", 0},
            {"ME.1.20", 0},
            {"ME.1.21", 0},
            // 2. row: background B
            {"ME.2.1", 0},
            {"ME.2.2", 0},
            {"ME.2.3", 0},
            {"ME.2.4", 0},
            {"ME.2.5", 0},
            {"ME.2.6", 0},
            //    {"ME.2.7", 0},
            //    {"ME.2.8", 0},
            //    {"ME.2.9", 0},
            //    {"ME.2.10", 0},
            //    {"ME.2.11", 0},
            //    {"ME.2.12", 0},
            //    {"ME.2.13", 0},
            //    {"ME.2.14", 0},
            //    {"ME.2.15", 0},
            //    {"ME.2.16", 0},
            //    {"ME.2.17", 0},
            //    {"ME.2.18", 0},
            //    {"ME.2.19", 0},
            //    {"ME.2.20", 0},
            //    {"ME.2.21", 0},
            // 3. row: background a
            {"ME.3.1", 0},
            {"ME.3.2", 0},
            {"ME.3.3", 0},
            {"ME.3.4", 0},
            {"ME.3.5", 0},
            {"ME.3.6", 0},
            //    {"ME.3.7", 0},
            //    {"ME.3.8", 0},
            //    {"ME.3.9", 0},
            //    {"ME.3.10", 0},
            //    {"ME.3.11", 0},
            //    {"ME.3.12", 0},
            //    {"ME.3.13", 0},
            //    {"ME.3.14", 0},
            //    {"ME.3.15", 0},
            //    {"ME.3.16", 0},
            //    {"ME.3.17", 0},
            //    {"ME.3.18", 0},
            //    {"ME.3.19", 0},
            //    {"ME.3.20", 0},
            //    {"ME.3.21", 0},

            /**
                * PP
            */
            // special keys
            //    {"PP.2nd", 0},
            // 1. row: key busses
            {"PP.1.1", 0},
            {"PP.1.2", 0},
            {"PP.1.3", 0},
            {"PP.1.4", 0},
            {"PP.1.5", 0},
            {"PP.1.6", 0},
            //    {"PP.1.7", 0},
            //    {"PP.1.8", 0},
            //    {"PP.1.9", 0},
            //    {"PP.1.10", 0},
            //    {"PP.1.11", 0},
            //    {"PP.1.12", 0},
            //    {"PP.1.13", 0},
            //    {"PP.1.14", 0},
            //    {"PP.1.15", 0},
            //    {"PP.1.16", 0},
            //    {"PP.1.17", 0},
            //    {"PP.1.18", 0},
            //    {"PP.1.19", 0},
            //    {"PP.1.20", 0},
            //    {"PP.1.21", 0},
            // 2. row: programm
            {"PP.2.1", 0},
            {"PP.2.2", 0},
            {"PP.2.3", 0},
            {"PP.2.4", 0},
            {"PP.2.5", 0},
            {"PP.2.6", 0},
            //    {"PP.2.7", 0},
            //    {"PP.2.8", 0},
            //    {"PP.2.9", 0},
            //    {"PP.2.10", 0},
            //    {"PP.2.11", 0},
            //    {"PP.2.12", 0},
            //    {"PP.2.13", 0},
            //    {"PP.2.14", 0},
            //    {"PP.2.15", 0},
            //    {"PP.2.16", 0},
            //    {"PP.2.17", 0},
            //    {"PP.2.18", 0},
            //    {"PP.2.19", 0},
            //    {"PP.2.20", 0},
            //    {"PP.2.21", 0},
            //    // 3. row: preset
            {"PP.3.1", 0},
            {"PP.3.2", 0},
            {"PP.3.3", 0},
            {"PP.3.4", 0},
            {"PP.3.5", 0},
            {"PP.3.6", 0},
            //    {"PP.3.7", 0},
            //    {"PP.3.8", 0},
            //    {"PP.3.9", 0},
            //    {"PP.3.10", 0},
            //    {"PP.3.11", 0},
            //    {"PP.3.12", 0},
            //    {"PP.3.13", 0},
            //    {"PP.3.14", 0},
            //    {"PP.3.15", 0},
            //    {"PP.3.16", 0},
            //    {"PP.3.17", 0},
            //    {"PP.3.18", 0},
            //    {"PP.3.19", 0},
            //    {"PP.3.20", 0},
            //    {"PP.3.21", 0},

            /**
                * Wipe
            */
            // fat block
            {"Wipe.A", 0},
            {"Wipe.B", 0},
            {"Wipe.C", 0},
            {"Wipe.D", 0},
            {"Wipe.E", 0},
            {"Wipe.F", 0},
            {"Wipe.G", 0},
            {"Wipe.H", 0},
            {"Wipe.I", 0},
            {"Wipe.J", 0},
            {"Wipe.K", 0},
            {"Wipe.L", 0},
            {"Wipe.M", 0},
            {"Wipe.N", 0},
            {"Wipe.O", 0},
            {"Wipe.P", 0},
            {"Wipe.Q", 0},
            {"Wipe.R", 0},
            {"Wipe.S", 0},
            {"Wipe.T", 0},
            {"Wipe.U", 0},
            {"Wipe.V", 0},
            {"Wipe.W", 0},
            {"Wipe.X", 0},
            {"Wipe.Y", 0},
            {"Wipe.Z", 0},
            {"Wipe.+", 0},
            {"Wipe.-", 0},
            {"Wipe.*", 0},
            {"Wipe./", 0},
            {"Wipe.U1", 0},
            {"Wipe.U2", 0},
            {"Wipe.U3", 0},
            {"Wipe.U4", 0},
            {"Wipe.U5", 0},
            // around block
            {"Wipe.ext_pos", 0},
            {"Wipe.pos", 0},
            {"Wipe.rot_user", 0},
            {"Wipe.text", 0},
            {"Wipe.flipflop", 0},
            {"Wipe.rev", 0},
            {"Wipe.ME2", 0},
            {"Wipe.ME1", 0},
            // above poti
            {"Wipe.1", 0},
            {"Wipe.2", 0},
            {"Wipe.3", 0},
            {"Wipe.4", 0},
            // below poti
            {"Wipe.mod", 0},
            {"Wipe.multi", 0},
            {"Wipe.wipe_comb", 0},
            {"Wipe.wipe_rot", 0},
            {"Wipe.board", 0},
            {"Wipe.soft", 0},
            {"Wipe.aspect", 0},
            {"Wipe.matrix", 0},
            // colored shit and above
            {"Wipe.mod_reset", 0},
            {"Wipe.DVE1", 0},
            {"Wipe.DVE2", 0},
            {"Wipe.ME1_w1", 0},
            {"Wipe.ME1_w2", 0},
            {"Wipe.ME2_w1", 0},
            {"Wipe.ME2_w2", 0},
            {"Wipe.DSK_w1", 0},
            {"Wipe.DSK_w2", 0},

            /**
                * to dve
            */
            {"tDVE.loop", 0},
            {"tDVE.preset", 0},
            {"tDVE.pgm", 0},
            {"tDVE.key1", 0},
            {"tDVE.key2", 0},
            {"tDVE.me1", 0},
            {"tDVE.me2", 0},
            {"tDVE.pp", 0},

            /**
                * delegation
            */
            {"deleg.dve1", 0},
            {"deleg.dve2", 0},
            {"deleg.aux1", 0},
            {"deleg.aux2", 0},
            {"deleg.aux3", 0},
            {"deleg.aux4", 0},
            {"deleg.aux5", 0},
            //    {"deleg.aux6", 0},    
            //    {"deleg.dve1", 0},    
            //    {"deleg.dve1", 0},    
            //    {"deleg.dve1", 0},    
            //    {"deleg.dve1", 0},

            /**
                * transition ME
            */
            {"tME.limit_set", 0},
            {"tME.limit_on", 0},
            {"tME.bgdl", 0},
            {"tME.bgdr", 0},
            {"tME.key1", 0},
            {"tME.key2", 0},
            {"tME.add", 0},
            {"tME.bve", 0},
            {"tME.mix", 0},
            {"tME.wipe1", 0},
            {"tME.wipe2", 0},
            {"tME.blk_pst", 0},
            {"tME.trans_dur", 0},
            {"tME.trans_pvw", 0},
            {"tME.auto", 0},
            {"tME.cut", 0},
            /**
                * transition PP
            */
            {"tPP.limit_set", 0},
            {"tPP.limit_on", 0},
            {"tPP.bgdl", 0},
            {"tPP.bgdr", 0},
            {"tPP.dsk1", 0},
            {"tPP.dsk2", 0},
            {"tPP.add", 0},
            {"tPP.bve", 0},
            {"tPP.mix", 0},
            {"tPP.wipe1", 0},
            {"tPP.wipe2", 0},
            {"tPP.blk_pst", 0},
            {"tPP.trans_dur", 0},
            {"tPP.trans_pvw", 0},
            {"tPP.auto", 0},
            {"tPP.cut", 0},

            /**
                * Keyers
            */
            {"key.shdw_store", 0},
            {"key.bord", 0},
            {"key.shdw", 0},
            {"key.drop", 0},
            {"key.outline", 0},
            {"key.key_over", 0},
            {"key.key_inv", 0},
            {"key.mask_on", 0},
            {"key.mask_key", 0},
            {"key.key_pvw", 0},
            {"key.matte", 0},
            {"key.key_bus_l", 0},
            {"key.key_bus_r", 0},
            {"key.cupl_split", 0},
            {"key.add_lin", 0},
            {"key.lum_lin", 0},
            {"key.chr_key", 0},
            {"key.pattn1", 0},
            {"key.pattn2", 0},
            {"key.cut", 0},
            {"key.bgda", 0},
            {"key.bgdb", 0},
            {"key.key1", 0},
            {"key.key2", 0},
            {"key.auto", 0},

            /**
                * DSK
            */
            {"dsk.coupl", 0},
            {"dsk.trans_dur_l", 0},
            {"dsk.trans_dur_r", 0},
            {"dsk.cut_l", 0},
            {"dsk.cut_r", 0},
            {"dsk.auto_l", 0},
            {"dsk.auto_r", 0},
            {"dsk.shdw_store", 0},
            {"dsk.bord", 0},
            {"dsk.shdw", 0},
            {"dsk.drop", 0},
            {"dsk.outline", 0},
            {"dsk.key_over", 0},
            {"dsk.key_inv", 0},
            {"dsk.mask_on", 0},
            {"dsk.mask_key", 0},
            {"dsk.key_pvw", 0},
            {"dsk.matte", 0},
            {"dsk.key_bus_l", 0},
            {"dsk.key_bus_r", 0},
            {"dsk.cupl_split", 0},
            {"dsk.add_lin", 0},
            {"dsk.lum_lin", 0},
            {"dsk.chr_key", 0},
            {"dsk.pattn1", 0},
            {"dsk.pattn2", 0},
            {"dsk.bgda", 0},
            {"dsk.bgdb", 0},
            {"dsk.dsk1", 0},
            {"dsk.dsk2", 0},
            {"dsk.auto", 0},
            //
            //    /**
            //     * Chroma Keyers
            //    */
            //    {-1 ,"chr.key_color"},
            //    {-1 ,"chr.unknown"},
            //    {-1 ,"chr.auto"},
            //    {-1 ,"chr.fgd"},
            //    {-1 ,"chr.me1_key1"},
            //    {-1 ,"chr.me1_key2"},
            //    {-1 ,"chr.me2_key1"},
            //    {-1 ,"chr.me2_key2"},
            //    {-1 ,"chr.dsk1"},
            //    {-1 ,"chr.dsk2"},
            //
            //    /**
            //     * Positioner
            //    */
            //    {-1 ,"pos.h_lock"},
            //    {-1 ,"pos.cent"},
            //    {-1 ,"pos.v_lock"},
            //
            //    /**
            //     * Extra
            //    */
            //    {-1 ,"extra.above.pvw_me1"},
            //    {-1 ,"extra.above.pvw_me2"},
            //    {-1 ,"extra.above.pvw_pp"},
            //    {-1 ,"extra.above.clean"},
            //    {-1 ,"extra.flip_flop"},
            //    {-1 ,"extra.rev"},
            //    {-1 ,"extra.stop_next"},
            //    {-1 ,"extra.stop"},
            //    {-1 ,"extra.me1"},
            //    {-1 ,"extra.col_bgd"},
            //    {-1 ,"extra.store"},
            //    {-1 ,"extra.bank"},
            //    {-1 ,"extra.edit"},
            //    {-1 ,"extra.me2"},
            //    {-1 ,"extra.misc"},
            //    {-1 ,"extra.pp"},
            //    {-1 ,"extra.reloc"},
            //    {-1 ,"extra.7"},
            //    {-1 ,"extra.8"},
            //    {-1 ,"extra.9"},
            //    {-1 ,"extra.4"},
            //    {-1 ,"extra.5"},
            //    {-1 ,"extra.6"},
            //    {-1 ,"extra.1"},
            //    {-1 ,"extra.2"},
            //    {-1 ,"extra.3"},
            //    {-1 ,"extra.clear"},
            //    {-1 ,"extra.0"},
            //    {-1 ,"extra.enter"},
            //    {-1 ,"extra.undo"},
            //    {-1 ,"extra.cut"},
            //    {-1 ,"extra.auto"},
            //    {-1 ,"extra.trans_dur"},
            //    
            //    /*
            //     * ftb
            //    */
            //    {-1 ,"ftb.ftb"},
            //    {-1 ,"ftb.trans_dur"},
            //    {-1 ,"ftb.cut"},
            //    {-1 ,"ftb.auto"},
            //
            //    /*
            //     * Display
            //    */
            //    {-1 ,"display.vtr_enable"},
            //    {-1 ,"display.edit_enable"},
            //    {-1 ,"display.gpi_enable"},
            //    {-1 ,"display.me1"},
            //    {-1 ,"display.me2"},
            //    {-1 ,"display.pp"},
            //    {-1 ,"display.status"},
            //    {-1 ,"display.transf"},
            //    {-1 ,"display.extra"},
            //    {-1 ,"display.gpi"},
            //    {-1 ,"display.access"},
            //    {-1 ,"display.key"},
            //    {-1 ,"display.mont_prog"},
            //    {-1 ,"display.install"},
            //    {-1 ,"display.stores"},
            //    {-1 ,"display.masks"},
            //    {-1 ,"display.exit"},
            //    {-1 ,"display.second"},
            //    {-1 ,"display.above.1"},
            //    {-1 ,"display.above.2"},
            //    {-1 ,"display.above.3"},
            //    {-1 ,"display.above.4"},
            //    {-1 ,"display.above.5"},
            //    {-1 ,"display.above.6"},
            //    {-1 ,"display.above.7"},
            //    {-1 ,"display.above.8"},
            //    {-1 ,"display.above.9"},
            //    {-1 ,"display.above.10"},
            //    {-1 ,"display.below.1"},
            //    {-1 ,"display.below.2"},
            //    {-1 ,"display.below.3"},
            //    {-1 ,"display.below.4"},
            //    {-1 ,"display.below.5"},
            //    {-1 ,"display.below.6"},
            //    {-1 ,"display.below.7"},
            //    {-1 ,"display.below.8"},
            //    {-1 ,"display.below.9"},
            //    {-1 ,"display.below.10"},
        };
}
