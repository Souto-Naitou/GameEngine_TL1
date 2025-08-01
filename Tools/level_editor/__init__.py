import bpy

import gpu
import gpu_extras.batch
import mathutils
import math
import copy

from .stretch_vertex import MYADDON_OT_stretch_vertex
from .create_ico_sphere import MYADDON_OT_create_ico_sphere
from .export_scene import MYADDON_OT_export_scene
from .my_menu import TOPBAR_MT_my_menu
from .add_filename import MYADDON_OT_add_filename
from .draw_collider import DrawCollider
from .add_collider import MYADDON_OT_add_collider
from .remove_collider import MYADDON_OT_remove_collider
from .pt_collider import OBJECT_PT_collider
from .add_uvtiling import MYADDON_OT_add_uvtiling
from .remove_uvtiling import MYADDON_OT_remove_uvtiling
from .pt_uvtiling import MATERIAL_PT_uvtiling
from .pt_file_name import OBJECT_PT_file_name
from .connect_engine import MYADDON_OT_connect_engine, MYADDON_OT_disconnect_engine 

# ブレンダーに登録する情報
bl_info = {
    "name": "レベルエディタ",
    "author": "Souto Naitou",
    "version": (1, 0),
    "blender": (3,3,1),
    "location": "",
    "description": "レベルエディタ",
    "warning" : "",
    "wiki_url" : "",
    "tracker_url" : "",
    "category" : "Object"
}

# URLを開くオペレータを定義した関数
def draw_menu_manual(self, context):
    # トップバーの「エディターメニュー」に項目(オペレータ)を追加
    self.layout.operator("wm.url_open_preset", text="Manual", icon="HELP")

# Blenderに登録するクラスリスト
classes = (
    MYADDON_OT_stretch_vertex,
    MYADDON_OT_create_ico_sphere,
    MYADDON_OT_export_scene,
    TOPBAR_MT_my_menu,
    MYADDON_OT_add_filename,
    OBJECT_PT_file_name,
    MYADDON_OT_add_collider,
    MYADDON_OT_remove_collider,
    OBJECT_PT_collider,
    MYADDON_OT_add_uvtiling,
    MYADDON_OT_remove_uvtiling,
    MATERIAL_PT_uvtiling,
    MYADDON_OT_connect_engine,
    MYADDON_OT_disconnect_engine,
)


# 有効化時のコールバック
def register():
    # クラスを登録
    for cls in classes:
        bpy.utils.register_class(cls)

    # メニューに追加
    bpy.types.TOPBAR_MT_editor_menus.append(TOPBAR_MT_my_menu.submenu)
    # 3Dビューに描画ハンドルを追加
    DrawCollider.handle = bpy.types.SpaceView3D.draw_handler_add(
            DrawCollider.draw_collider, (), 'WINDOW', 'POST_VIEW')

    print("レベルエディタが有効化されました。")


# 無効化時のコールバック
def unregister():
    # メニューから削除
    bpy.types.TOPBAR_MT_editor_menus.remove(TOPBAR_MT_my_menu.submenu)
    # 3Dビューから描画ハンドルを削除
    bpy.types.SpaceView3D.draw_handler_remove(DrawCollider.handle, 'WINDOW')

    # Unregister classes
    for cls in classes:
        bpy.utils.unregister_class(cls)

    print("レベルエディタが無効化されました。")


# main関数
if __name__ == "__main__":
    register()