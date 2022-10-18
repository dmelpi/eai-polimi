/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Tue Oct 11 17:42:42 2022
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "network_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_network_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_network_weights_array_u64[421] = {
  0x3ec5707c3d8c3e53U, 0x3f429a733f1e2229U, 0x3fa4b12a3f7dad66U, 0x3fd54fd23fb35718U,
  0x40031d003fddc8ecU, 0x402670ab402acd5eU, 0xbecc16c0bf79bb6bU, 0xbe59605f3eb48e1bU,
  0x3e7bfe593f045669U, 0x3e3a35b23eba0f78U, 0x3b7365783e34ee98U, 0x3d24d8653e9a0601U,
  0xbda97351bd063c29U, 0x3d8b3165bd075b36U, 0x3d02e6893b1782f1U, 0xbd849b9abcae2422U,
  0xb8fd6cd03b316920U, 0xb9f9452ebcf268ebU, 0x3d301e5cbbe68819U, 0x3cb601303c88e050U,
  0x3d3bd1143dbd24bcU, 0x3dc50034bb40c42dU, 0x3d89e517bdb9258fU, 0xbd20d714bb961606U,
  0xbda8d336bdd4c8cdU, 0x3d19e0573dd0f773U, 0xbca26b353d147153U, 0xbd781d56bd2db951U,
  0xbd4dace0bcb06182U, 0xbcefcabebd472350U, 0xbddfb0273ddddb80U, 0x3c9332163b9dead5U,
  0xbc86778f3cae7a4eU, 0x3d99ec95bd257f00U, 0xbdd3dafcbce4a362U, 0x3e01a95cbd2e1992U,
  0xbc88b4dc3dd1028fU, 0x3d0ceb0abd119f7cU, 0xbdaa6a133d8358bdU, 0x3c3332723b5ddbd5U,
  0x3c91fc6b3ca3ced4U, 0x3c62317e3c217d96U, 0xbc05d60f39efdd1fU, 0x3d133d5a3d5c37eeU,
  0x3c46e7783cf9ea91U, 0x3cc57a243cc3a0faU, 0xbd15e831bccd4a03U, 0x3ade10d8bcf90cb2U,
  0x3d1b10fd3c3cc554U, 0x3ae2a5633cec75eeU, 0xbd118d6d3b480ffdU, 0xbd4739353cc6d668U,
  0x3b90c1e4bd223678U, 0xbb5e3a4a3ceb1862U, 0x3b8187cb3ce10eeaU, 0x3d4096773d4407c9U,
  0x3cb60e913c0db0bcU, 0x3cc0a9f53cb138a8U, 0xbdbc3c663d18ba60U, 0x3bea7cfdbd8cb82aU,
  0xbd118167bb52865fU, 0x3cdcb4a13ce522e8U, 0xbc9421183bdff5c8U, 0x3bf10fb23d422bacU,
  0x3b9040adbced5778U, 0xbc5192a93b94a5b3U, 0xbbe6224fbb9efccbU, 0x3a6a1630bc6e7d55U,
  0xbbe813c6bc4a4937U, 0xbbc4ec723a5c75adU, 0x3b62f87fbbd5336eU, 0xbc1b3810bcc93161U,
  0xbd317160bcd12a84U, 0x3a231114bc408e4dU, 0xbbcf39f83c4186d5U, 0xbd17046f3d62562eU,
  0xbd87ce89bb87f519U, 0x3bd0cda13d8fa95eU, 0xbce6e2dc3d056c22U, 0x3d74f9f33a801dc4U,
  0x3b14076bbd32df9bU, 0x3bf1be37bc3381d9U, 0xbbae95e7bd0036a5U, 0xbd73b4cb3cf40499U,
  0x3d93321e3d5db280U, 0x3cf3c7e53d0ab1beU, 0xbbf8be203c2e8336U, 0xbdb80dc9bdaa290cU,
  0xbaee8736bd3b81a5U, 0xbcc3169d3dc5238eU, 0x3bae1d64bc9d6a9eU, 0x3d8ad276bb45a989U,
  0x3d8eebb1bd7094bfU, 0xbca284ffbcc55256U, 0xbaff6c0ebc9dd5b3U, 0x3a0dfdfe3bb125fdU,
  0x3cc0919e3cd0d3cbU, 0x3d0a33cc3d09c22cU, 0x3c3f41d63c7bba93U, 0x3cb43eb53ce99a8aU,
  0x3c66ec2d3cab5339U, 0x3c40a0133c3d215bU, 0x3d4fe7b5bd4de3b0U, 0x3d248d683c1a21feU,
  0x3caed04dbd97f1d4U, 0x3d027ab13c58c676U, 0xbcdab3e8bcdbcc0fU, 0xbd4b92cdbd393c87U,
  0xbc60da1bbc6b3fa6U, 0x3d4257bebc8522f2U, 0x3d5585033d537981U, 0x3d0a68483d396d09U,
  0xbc0e888c3d8a9db1U, 0xbc812ec03b3193d9U, 0xbd5a4e01bcc25a13U, 0xbd86083dbd8765f3U,
  0xbbd6c3793d123e44U, 0xbc4f9f7dbb851353U, 0xbb83d2143b8daa39U, 0x3a5840bebc973e99U,
  0x3d5307b53dda698fU, 0x3b32eb8e3c5351e1U, 0xbd42e530bbf91372U, 0x3cb7b30e3bd91847U,
  0x3d9e2c4bbb173c22U, 0x3d0ad850bd5a832fU, 0xbdbfa45fbd700c29U, 0xbb4da011bd525059U,
  0xbdf258a7bd2bf67cU, 0xbdbe19b53dae16a6U, 0xbd68ae93bd6cecbaU, 0x3c1db50bbd73430eU,
  0x4125008541038db4U, 0x4130a77c412c5370U, 0x416e37394172ffdaU, 0x41a1802241548b9fU,
  0x4123c27c41800ef3U, 0x4147171d41518acfU, 0x41d2ffe641e5c024U, 0x415a9da541bc6977U,
  0x41ab1499418889c2U, 0x41d1653a41896450U, 0x41a9a55c41d5a105U, 0x4150d55d41a93462U,
  0xbf539c32bf80f054U, 0xbf08ada0bf24e52cU, 0xbed5e041bf258462U, 0xbf4c042bbf2b3c40U,
  0xbf887111bf09debcU, 0xbf2d8be6bf09d685U, 0xbe5a2aaebcead7a1U, 0xbf165558be82453dU,
  0xbe9bf810bef72fe9U, 0xbe5e2cabbfd628f4U, 0xbeea2811be98fcc9U, 0xbf1b3b2cbebe64c7U,
  0xbcf08c223c8ad672U, 0x3ba1de24bbd4f0afU, 0xbc85a4c2bd021fedU, 0xbd18590ebc09703eU,
  0x3bb4da1abc61995aU, 0xbcc45458bd199f5eU, 0xbd0356be3c3c7813U, 0xbb51acdcbcea5b64U,
  0xbcbc2460bcf33c24U, 0x3d84a35e3b2cee4bU, 0xbb9f9175bc040812U, 0x3cfdc4c23ce3f9e3U,
  0x3c55bd0abc9befbdU, 0xbbb88a66bc0d6cc9U, 0xbc82d0c0bc6fd240U, 0x3d05c1afbca624f3U,
  0xba72584cbcaaa810U, 0xbc248801bc91a21fU, 0xbd0e43e53d0583acU, 0xbc3500b0bd1379beU,
  0xbcacfec5bce78354U, 0xbd43a0c3bce1a1b5U, 0xbc0b0524bcbfb0e6U, 0xbd04c2f4bd072001U,
  0xbca903673c8775b0U, 0xbbddd661bd40f480U, 0xbd210096bca43231U, 0xbc42b2f6bd17216bU,
  0xbd2888263c74f144U, 0x3bb0bb77bca77a4bU, 0xbbef5be5bcbb9bd7U, 0xbd8e6ef53c42bb45U,
  0xbcaa22eb3c07fb6aU, 0xbc9240b0bcd5fb23U, 0xbd3537f1bc01ab33U, 0x3d83c1afbbcdb606U,
  0xbd227c463d2bad63U, 0xbbfada00bcd21f50U, 0xbcabd1ddbceb564cU, 0xbd214b8cbcce96baU,
  0xbb8c9856bc8715a1U, 0xbce56010bd0c5e67U, 0xbcaec7423cb434eeU, 0xba6df10cbcd916cdU,
  0xbcce687ebc81daeeU, 0x3d4dfeb8bb5ce184U, 0x395f8e4cbd3c9560U, 0xbd560fe2bcbe87f4U,
  0xbd428a823d576a30U, 0x3bf34b38bd78dc96U, 0xbd5032493c0b8400U, 0x3c6da4e0bc63de6eU,
  0x3af84f6dbd46d6c8U, 0xbd82f1eebbd1e088U, 0x3b9a0d3e3c2fc227U, 0x3b2ced14bc1d137aU,
  0xbc7be565babac5c7U, 0xbb7491663c73b28dU, 0x3cbb1d0cbb77ff95U, 0xbbec29403bc3f206U,
  0x3b0033443d049efbU, 0x3ce02153bbc4d000U, 0xbbab42df3b755285U, 0x3c97c90f3b0a3562U,
  0x3c9210733bd2a2faU, 0x3c9e8b08bc2d37dbU, 0x3c3d7d253c8fb9a5U, 0x3ca483ba3be091b1U,
  0x3c8c3aa637fbbbd8U, 0x3bb2f6463a3d7fb9U, 0x3ccadfb83c5ab828U, 0x3cfd69b1bc16bd41U,
  0x3d21a7bdbc9a5782U, 0xbc4a81003d10993aU, 0x3d391894bc49817eU, 0x3c9c1ae4bb9a0edfU,
  0xbcb0b99abcc26dddU, 0x3bc9b3a13d068e8cU, 0xbd38ecd5bbd5a37aU, 0xbcb8e3b0bd51870dU,
  0xbcf9a3d23c850187U, 0xbc77e38a3c04879bU, 0xbc43d79dbd70e79cU, 0xbd495ba63d057b92U,
  0xbcf430cebb5c3b64U, 0x3b9745a1bcc2233aU, 0xbc3c103bbb5ca947U, 0x3aed5b3ebc3ed676U,
  0xbbae69bd3d76b714U, 0x3d18d30ebd515af1U, 0xbb9921ad3c4010ebU, 0xb9e513b1bc033d5dU,
  0xbc2f9714bb05ffe3U, 0xbb7f74253cc5e811U, 0x3cb09353bb34f425U, 0xbbcecf213b7a4d1dU,
  0xbaaaf6363ca4fa2fU, 0x3c2f900dbbd9e0dcU, 0xbaf0d8be3b71f89eU, 0x3cd62695bbf2b5b6U,
  0xbbe04e843d0cec21U, 0x3cf0afbcbbe807abU, 0x3cfa2da43b285d9aU, 0xbc8e7ac3bd0a843bU,
  0x3c9fb7f63d0e312bU, 0xbd0d1dd5bba04803U, 0xbc936257bd410cb4U, 0xbcdb2d2c3ca85723U,
  0x3c58aea4bb1b0ae5U, 0x3af13740bba05437U, 0x3d171fb83cbe7b9aU, 0xbbd4d069bb815442U,
  0xba5e79343b8e5652U, 0xbc0d6c14bc927851U, 0xbc9ae3abbbfc63b4U, 0xbbd6b8a83d3cb728U,
  0x3d047a4b3b84b425U, 0xbc912fd2bbcf1f04U, 0x3c5e748e3b6f7093U, 0xbc915d35bcdabd15U,
  0xbc8eac15bb55e8bcU, 0xbc73638d3c2f381cU, 0x3cf589cebd3a9f45U, 0xba6408b7bae4ee93U,
  0x3c9574ce3d1a56bbU, 0xbc9f88e7bd079660U, 0xbb24be36bb590683U, 0xbcd06542bafc8f91U,
  0xbc9e712db982df2cU, 0xbc03dfdabb29ae80U, 0xbce49711bb5d4372U, 0xbc187fdc39db162bU,
  0xbbf97876bb20238fU, 0xbcaeed3aba8965c2U, 0xbc8c0724bb42142eU, 0xbc3dc124bc001117U,
  0xbd0687b2bcc53eafU, 0xbcad75fcbb07563cU, 0xbcc7a108bc17396cU, 0xbd08ae48bc5e72a7U,
  0xbca3bc67bb04e243U, 0xbc6089aabbcd4a27U, 0xbcfdac41bc4ab114U, 0xbcbbc4f7ba64b4c1U,
  0xbcb6901cbc0b4b13U, 0xbc1d62e2bbaf28ecU, 0x3d05f28dbd465ba5U, 0xbc7eac563a2bef2bU,
  0x3d104d2e3d039881U, 0xbce91301bd5b1467U, 0xbc280e30bb8c6498U, 0xbc2fc3f8bd00ec08U,
  0x3cd5c8f6bd19657dU, 0xbc9186c4bb907e8bU, 0xbb4e216dbc1eafdfU, 0x3c91c914bd0a0676U,
  0xbc8b2fb93a1cad26U, 0x3cba4e2f3cf25d66U, 0xbc8b6ad8bd204acaU, 0xbbef66db3ab2d7bbU,
  0xbbf60641bc923fabU, 0x3ccb5febbce80f2eU, 0xbbab2b82bb815692U, 0xbcfc59a4bc56f864U,
  0x3ab919dfbcec9b7aU, 0xbb00f3a3ba092486U, 0xbcec3aa83cb12479U, 0x3c71abecbc9e6418U,
  0xbc12f0233ac36daaU, 0xbcadf39abc32100aU, 0x3aec05c0bcd83021U, 0x85b026ff0a5066c9U,
  0xd122bdc070f4495U, 0x86ca622606aeeb87U, 0x86609c520804915eU, 0x86d8d5ee891e5c54U,
  0x868bc0098e35af95U, 0x6ba554926858443U, 0x5f6173686dd3e3aU, 0x853a3f9c06e36ac3U,
  0xbc97f065ba1eddbbU, 0xbd1cd42b3d136a0cU, 0x3c4de02abd34afcfU, 0xbc9f8d91bba6bce4U,
  0xbd1a4c11bc9a1bc7U, 0xbc0a4b0bbccaf0c9U, 0xbc5edbc83aa38d50U, 0xbd2f10d03d2212b4U,
  0x3c166371bd21a7dfU, 0xbc372bb23bbcc383U, 0x3cc112dabccca6d2U, 0xbce3dcf3bcdf9a35U,
  0xbd074eccbc8c19c7U, 0xbc2eadb1bcc6cd32U, 0xbce81d433ba60cb0U, 0xbcc3f799bd113b9eU,
  0xbc9b3e25bc8ba630U, 0xbb0ed19b3d07a2bcU, 0xbabf9e6a3c6c183dU, 0x3c1feb2bbc039afaU,
  0xbcde5250bcad7c54U, 0xbc7a6d7abb0c89d8U, 0xbc42d270bc3a05f6U, 0xbce80e9b3b32b7d8U,
  0xbc36aa03bcc9082cU, 0xbc420484bc09e9ffU, 0xbc7671ea3caca9dcU, 0x3bacb1963c33e0b9U,
  0x3cada227bc89da8cU, 0xbc60e2bcbc8a2ac3U, 0xbc45d5383c0e6b64U, 0x37be3400bc3bf93cU,
  0xbce45519b99227e4U, 0xbbff75e7bcf1aa1bU, 0xbc1afa62bc34e5f2U, 0xbc6780243d00d853U,
  0xbe7153e6bd125c38U, 0x3c9e5610bd582551U, 0xbd77a834bdad6f4dU, 0xbdb053833d159178U,
  0xbdad94d8bd81527eU, 0xbd38d09cbbf1d6fdU, 0xbbae136bbd30ef97U, 0x3cd562253cf7bea5U,
  0x3c90d5d0bcf509d4U, 0xbd1fc3f73d1bbcccU, 0xbce656843bb80b84U, 0x3b6c3f973bb2f4a3U,
  0x41bb70db4185851aU, 0x414348b3418ccd93U, 0x419357824199e56eU, 0x415b0259411dd1e9U,
  0x418802124158cbf0U, 0x4173c6bf417006c7U, 0x41c183d64184cacaU, 0x419e095e41af3c4cU,
  0x418e566c4192c84eU, 0x41f95985419c0722U, 0x419b39ce417f7e58U, 0x419a7b3b41998cacU,
  0xbe08312bbf3a7fe9U, 0xbf5ce12cbf51331bU, 0xbf427050bf104072U, 0xbeb28860bf1cb6b7U,
  0xbebdf394befa09d4U, 0xbf0072e0befa11f8U, 0xbf3f06acbe917ff9U, 0xbf8e9835bf83cddeU,
  0xbf324cc0bf122867U, 0xbd7e2111bf7ed0e9U, 0xbf1692d8bf4ee51dU, 0xbf29d5e8bf2329ddU,
  0xbe9df5c4beb94897U, 0x3f20c110bf224151U, 0xbf3e6becbf37e9fcU, 0xbe235d7ebe849f08U,
  0x3f107e223f126d5aU, 0x3ed82811bde5e803U, 0xbe8c363fbec2cfc3U, 0x3eaab63a3dbfecdeU,
  0xbe8c02ccbf1eba8cU, 0x3c388c25be2c1a3eU, 0x3ea91824bf0d6c78U, 0x3e2a2ec63e2287d1U,
  0xbc27b285U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

