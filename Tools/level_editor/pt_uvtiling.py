import bpy

from .add_uvtiling import MYADDON_OT_add_uvtiling
from .remove_uvtiling import MYADDON_OT_remove_uvtiling

class MATERIAL_PT_uvtiling(bpy.types.Panel):
    bl_idname = "MATERIAL_PT_uvtiling"
    bl_label = "UV Tiling"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "material"

    def draw(self, context):
        if "uv_tiling" in context.object:
            self.layout.prop(context.object, '["uv_tiling"]', text="Tiling")
            self.layout.operator(MYADDON_OT_remove_uvtiling.bl_idname)
        else:
            self.layout.operator(MYADDON_OT_add_uvtiling.bl_idname)