/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 *               |  Heng Guo  <2085471348@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-08-30>
 * Last Modified : <2024-08-16>
 * ------------------------------------------------------------*/

/**
 * @time 2024-06-12 更新
 */
static SourceInfo
os_ubuntu_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "https://mirrors.aliyun.com/ubuntu"},
  {&Volcengine,    "https://mirrors.volces.com/ubuntu"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/ubuntu"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/ubuntu"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/ubuntu"},
  {&Tencent,       "https://mirrors.tencent.com/ubuntu"},
  {&Huawei,        "https://mirrors.huaweicloud.com/ubuntu"},
  {&Netease,       "https://mirrors.163.com/ubuntu"},
  {&Sohu,          "https://mirrors.sohu.com/ubuntu"}
};
def_sources_n(os_ubuntu);


void
os_ubuntu_getsrc (char *option)
{
  if (chsrc_check_file (OS_Ubuntu_SourceList_DEB822))
    {
      chsrc_view_file (OS_Ubuntu_SourceList_DEB822);
      return;
    }

  if (chsrc_check_file (OS_Apt_SourceList))
    {
      chsrc_view_file (OS_Apt_SourceList);
      return;
    }

  chsrc_error2 ("缺少源配置文件！但仍可直接通过 chsrc set ubuntu 来添加使用新的源");
  return;
}


/**
 * 此函数基本和 os_ubuntu_setsrc() 一致
 */
void
os_ubuntu_setsrc_for_deb822 (char *option)
{
  SourceInfo source;
  chsrc_yield_source (os_ubuntu);
  chsrc_confirm_source (&source);

  chsrc_backup (OS_Ubuntu_SourceList_DEB822);

  char *arch = chsrc_get_cpuarch ();
  char *cmd  = NULL;
  if (strncmp (arch, "x86_64", 6)==0)
    {
      cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/ubuntu/?@", source.url, "@g\' " OS_Ubuntu_SourceList_DEB822);
    }
  else
    {
      cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/ubuntu-ports/?@", source.url, "-ports@g\' " OS_Ubuntu_SourceList_DEB822);
    }

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
}

/**
 * @note 不同架构下换源不一样
 */
void
os_ubuntu_setsrc (char *option)
{
  chsrc_ensure_root ();

  if (chsrc_check_file (OS_Ubuntu_SourceList_DEB822))
    {
      chsrc_note2 ("将基于新格式换源");
      os_ubuntu_setsrc_for_deb822 (option);
      return;
    }

  bool sourcelist_exist = ensure_apt_sourcelist (OS_Is_Ubuntu);

  SourceInfo source;
  chsrc_yield_source (os_ubuntu);
  chsrc_confirm_source (&source);

  // 不存在的时候，用的是我们生成的无效文件，不要备份
  if (sourcelist_exist)
    {
      chsrc_backup (OS_Apt_SourceList);
    }

  char *arch = chsrc_get_cpuarch ();
  char *cmd  = NULL;
  if (0==strncmp (arch, "x86_64", 6))
    {
      cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/ubuntu/?@", source.url, "@g\' " OS_Apt_SourceList);
    }
  else
    {
      cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/ubuntu-ports/?@", source.url, "-ports@g\' " OS_Apt_SourceList);
    }

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
}

def_target(os_ubuntu);
