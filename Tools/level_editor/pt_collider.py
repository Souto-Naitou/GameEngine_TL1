import bpy

from .add_collider import MYADDON_OT_add_collider
from .remove_collider import MYADDON_OT_remove_collider

class OBJECT_PT_collider(bpy.types.Panel):
    bl_idname = "OBJECT_PT_collider"
    bl_label = "Collider"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    def draw(self, context):
        if "collider" in context.object:
            # すでにプロパティがあれば、プロパティを表示
            self.layout.prop(context.object, '["collider"]', text="Type")
            self.layout.prop(context.object, '["collider_center"]', text="Center")
            self.layout.prop(context.object, '["collider_size"]', text="Size")
            # 削除ボタンを追加
            self.layout.operator(MYADDON_OT_remove_collider.bl_idname)
        else:
            self.layout.operator(MYADDON_OT_add_collider.bl_idname)