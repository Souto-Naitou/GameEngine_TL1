import bpy
import mathutils

class MYADDON_OT_add_uvtiling(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_uvtiling"
    bl_label = "UV Tiling 追加"
    bl_description = "['uv_tiling']カスタムプロパティを追加します"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        # カスタムプロパティを追加
        context.object["uv_tiling"] = mathutils.Vector((1.0, 1.0))

        return {'FINISHED'}