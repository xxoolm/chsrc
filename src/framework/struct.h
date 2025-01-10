/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     :  struct.h
 * File Authors  :  Aoran Zeng   <ccmywish@qq.com>
 *               |   Heng Guo    <2085471348@qq.com>
 * Contributors  : Shengwei Chen <414685209@qq.com>
 *               |
 * Created On    : <2023-08-29>
 * Last Modified : <2024-12-18>
 *
 * chsrc struct
 * ------------------------------------------------------------*/

typedef struct SpeedMeasureInfo_t
{
  bool  skip;           /* 是否默认跳过 */
  char *skip_reason_CN; /* 跳过的原因（中文）*/
  char *skip_reason_EN; /* 跳过的原因（英文）*/
  char *url;            /* 测速链接 */
}
SpeedMeasureInfo_t;

#define SKIP    true
#define NotSkip false
#define ToFill  NULL
#define NA		  NULL

typedef struct SourceProvider_t
{
  const char *code; /* 用于用户指定某一 Provider */
  const char *abbr; /* 需要使用 Provider 的英文名时，用这个代替，因为大部分 Provider 没有提供正式的英文名 */
  const char *name; /* Provider 中文名 */
  const char *site; /* Provider 首页   */
  SpeedMeasureInfo_t smi;
}
SourceProvider_t;

typedef SourceProvider_t MirrorSite_t;

SourceProvider_t UpstreamProvider =
{
  /* 引入新的上游默认源时，请使下面第一行的前三个字段保持不变，只添加第四个字段，可使用 def_upstream 宏 */
  "upstream", "Upstream", "上游默认源", NULL,
  /* 引入新的上游默认源时，请完全修改下面这个结构体，可使用 def_need_measure_info 宏 */
  {SKIP, "URL未知，邀您参与贡献!", "URL unknown, welcome to contribute!", NULL}
};

#define def_upstream            "upstream", "Upstream", "上游默认源"
#define def_need_measure_info   {SKIP, "缺乏较大的测速对象，邀您参与贡献!", "Lack of large object URL, welcome to contribute!", NULL}

SourceProvider_t UserDefinedProvider =
{
  "user", "用户自定义", "用户自定义", NULL,
  {SKIP, "用户自定义源不测速", "SKIP for user-defined source", NULL}
};


typedef struct Source_t
{
  union {
    SourceProvider_t *provider;
    MirrorSite_t     *mirror;
  };
  char *url;
}
Source_t;

#define def_sources_n(t) const size_t t##_sources_n = xy_arylen(t##_sources)


enum Capability_t
{
  CanNot,
  FullyCan,
  PartiallyCan
};


/* Target Feature */
typedef struct Feature_t
{
  bool can_get;
  bool can_reset;       /* 有的reset不是暂时没有实现，而是现在的实现根本就无法重置 */

  bool can_english;

  bool can_user_define; /* 用户自定义换源URL */

  enum Capability_t cap_locally;
  char *cap_locally_explain;

  char *note;
}
Feature_t;


typedef struct Target_t
{
  void (*getfn)   (char *option);
  void (*setfn)   (char *option);
  void (*resetfn) (char *option);

  Feature_t (*featfn) (char *option);

  Source_t *sources;
  size_t    sources_n;
}
Target_t;


#define def_target_inner_s(t)    NULL,       t##_setsrc, NULL,         NULL
#define def_target_inner_sr(t)   NULL,       t##_setsrc, t##_resetsrc, NULL
#define def_target_inner_sf(t)   NULL,       t##_setsrc, NULL,         t##_feat
#define def_target_inner_srf(t)  NULL,       t##_setsrc, t##_resetsrc, t##_feat
#define def_target_inner_gs(t)   t##_getsrc, t##_setsrc, NULL,         NULL
#define def_target_inner_gsr(t)  t##_getsrc, t##_setsrc, t##_resetsrc, NULL
#define def_target_inner_gsf(t)  t##_getsrc, t##_setsrc, NULL,         t##_feat
#define def_target_inner_gsrf(t) t##_getsrc, t##_setsrc, t##_resetsrc, t##_feat

#define def_target_sourcesn(t)   t##_sources, t##_sources_n

/* 大部分target还不支持reset，所以暂时先默认设置为NULL来过渡 */
#define def_target(t)      Target_t t##_target = {def_target_inner_gs(t),def_target_sourcesn(t)}
#define def_target_s(t)    Target_t t##_target = {def_target_inner_s(t),def_target_sourcesn(t)}
#define def_target_sr(t)   Target_t t##_target = {def_target_inner_sr(t),def_target_sourcesn(t)}
#define def_target_sf(t)   Target_t t##_target = {def_target_inner_sf(t),def_target_sourcesn(t)}
#define def_target_srf(t)  Target_t t##_target = {def_target_inner_srf(t),def_target_sourcesn(t)}
#define def_target_gs(t)   Target_t t##_target = {def_target_inner_gs(t),def_target_sourcesn(t)}
#define def_target_gsr(t)  Target_t t##_target = {def_target_inner_gsr(t),def_target_sourcesn(t)}
#define def_target_gsf(t)  Target_t t##_target = {def_target_inner_gsf(t),def_target_sourcesn(t)}
#define def_target_gsrf(t) Target_t t##_target = {def_target_inner_gsrf(t),def_target_sourcesn(t)}
