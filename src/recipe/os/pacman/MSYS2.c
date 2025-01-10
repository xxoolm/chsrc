/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Heng Guo <2085471348@qq.com>
 * Contributors  : Nil Null <nil@null.org>
 * Created On    : <2023-09-06>
 * Last Modified : <2024-08-16>
 * ------------------------------------------------------------*/

/**
 * @update 2024-04-18
 */
static Source_t os_msys2_sources[] =
{
  {&UpstreamProvider,  NULL},
  {&Ali,              "https://mirrors.aliyun.com/msys2"},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/msys2"},
  {&Ustc,             "https://mirrors.ustc.edu.cn/msys2"},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/msys2"},
  {&Tencent,          "https://mirrors.tencent.com/msys2"},
  {&Huawei,           "https://mirrors.huaweicloud.com/msys2"},
  {&Netease,          "https://mirrors.163.com/msys2"},
  {&Sohu,             "https://mirrors.sohu.com/msys2"}
};
def_sources_n(os_msys2);


/**
 * HELP: 未经测试
 */
void
os_msys2_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (os_msys2);

  chsrc_backup ("/etc/pacman.d/mirrorlist.mingw32");
  chsrc_backup ("/etc/pacman.d/mirrorlist.mingw64");
  chsrc_backup ("/etc/pacman.d/mirrorlist.msys");

  char *prev = xy_strjoin (3, "请针对你的架构下载安装此目录下的文件:",
                              source.url,
                             "distrib/<架构>/");
  chsrc_note2 (prev);

  char *cmd = xy_strjoin (3, "sed -i \"s#https\?://mirror.msys2.org/#",
                              source.url,
                             "#g\" /etc/pacman.d/mirrorlist* ");

  chsrc_run (cmd, RunOpt_Default);

  ProgMode_ChgType = ChgType_Untested;
  chsrc_conclude (&source);
}

def_target_s(os_msys2);
