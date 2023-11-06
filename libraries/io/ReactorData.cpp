#include "ReactorData.h"

namespace io {

  enum volumeTypeGDML {
    NT_1,
    GC_82,
    BUFFER_506,
    NT_TANK,
    NT_GIRDER_34,
    NT_GIRDER_37,
    NT_GIRDER_40,
    NT_GIRDER_43,
    NT_GIRDER_46,
    NT_GIRDER_49,
    NT_RING,
    NT_STIFFENER_35,
    NT_STIFFENER_38,
    NT_STIFFENER_41,
    NT_STIFFENER_44,
    NT_STIFFENER_47,
    NT_STIFFENER_50,
    GC_TANK,
    GC_GIRDER_BOTTOM_95,
    GC_GIRDER_BOTTOM_99,
    GC_GIRDER_BOTTOM_103,
    GC_GIRDER_BOTTOM_107,
    GC_GIRDER_BOTTOM_111,
    GC_GIRDER_BOTTOM_115,
    GC_GIRDER_MIDDLE_94,
    GC_GIRDER_MIDDLE_98,
    GC_GIRDER_MIDDLE_102,
    GC_GIRDER_MIDDLE_106,
    GC_GIRDER_MIDDLE_110,
    GC_GIRDER_MIDDLE_114,
    GC_GIRDER_TOP_93,
    GC_GIRDER_TOP_97,
    GC_GIRDER_TOP_101,
    GC_GIRDER_TOP_105,
    GC_GIRDER_TOP_109,
    GC_GIRDER_TOP_113,
    GC_STIFFENER_92,
    GC_STIFFENER_96,
    GC_STIFFENER_100,
    GC_STIFFENER_104,
    GC_STIFFENER_108,
    GC_STIFFENER_112,
    volumeTypeMaxGDML
  };

  double nd_scaling_factor(int gdml) {
    switch (gdml) {
      case NT_1:
        return 1.00000;
      case GC_82:
        return 0.99622;
      case BUFFER_506:
        return 1.000;
      case NT_TANK:
        return 1.020;
      case NT_GIRDER_34:
      case NT_GIRDER_37:
      case NT_GIRDER_40:
      case NT_GIRDER_43:
      case NT_GIRDER_46:
      case NT_GIRDER_49:
        return 3.431;
      case NT_RING:
        return 1.769;
      case NT_STIFFENER_35:
      case NT_STIFFENER_38:
      case NT_STIFFENER_41:
      case NT_STIFFENER_44:
      case NT_STIFFENER_47:
      case NT_STIFFENER_50:
        return 1.074;
      case GC_TANK:
        return 1.127;
      case GC_GIRDER_BOTTOM_95:
      case GC_GIRDER_BOTTOM_99:
      case GC_GIRDER_BOTTOM_103:
      case GC_GIRDER_BOTTOM_107:
      case GC_GIRDER_BOTTOM_111:
      case GC_GIRDER_BOTTOM_115:
      case GC_GIRDER_MIDDLE_94:
      case GC_GIRDER_MIDDLE_98:
      case GC_GIRDER_MIDDLE_102:
      case GC_GIRDER_MIDDLE_106:
      case GC_GIRDER_MIDDLE_110:
      case GC_GIRDER_MIDDLE_114:
        return 1.051;
      case GC_GIRDER_TOP_93:
      case GC_GIRDER_TOP_97:
      case GC_GIRDER_TOP_101:
      case GC_GIRDER_TOP_105:
      case GC_GIRDER_TOP_109:
      case GC_GIRDER_TOP_113:
        return 2.829;
      case GC_STIFFENER_92:
      case GC_STIFFENER_96:
      case GC_STIFFENER_100:
      case GC_STIFFENER_104:
      case GC_STIFFENER_108:
      case GC_STIFFENER_112:
        return 1.213;
      default:
        throw std::invalid_argument("Volume could not be identified in the ND ROOT information");
    }
  }

  double fd1_scaling_factor(int gdml) {
    switch (gdml) {
      case NT_1:
        return 0.99881;
      case GC_82:
        return 0.99180;
      case BUFFER_506:
        return 1.0000;
      case NT_TANK:
        return 1.020;
      case NT_GIRDER_34:
      case NT_GIRDER_37:
      case NT_GIRDER_40:
      case NT_GIRDER_43:
      case NT_GIRDER_46:
      case NT_GIRDER_49:
        return 3.431;
      case NT_RING:
        return 1.769;
      case NT_STIFFENER_35:
      case NT_STIFFENER_38:
      case NT_STIFFENER_41:
      case NT_STIFFENER_44:
      case NT_STIFFENER_47:
      case NT_STIFFENER_50:
        return 1.074;
      case GC_TANK:
        return 1.124;
      case GC_GIRDER_BOTTOM_95:
      case GC_GIRDER_BOTTOM_99:
      case GC_GIRDER_BOTTOM_103:
      case GC_GIRDER_BOTTOM_107:
      case GC_GIRDER_BOTTOM_111:
      case GC_GIRDER_BOTTOM_115:
      case GC_GIRDER_MIDDLE_94:
      case GC_GIRDER_MIDDLE_98:
      case GC_GIRDER_MIDDLE_102:
      case GC_GIRDER_MIDDLE_106:
      case GC_GIRDER_MIDDLE_110:
      case GC_GIRDER_MIDDLE_114:
        return 1.047;
      case GC_GIRDER_TOP_93:
      case GC_GIRDER_TOP_97:
      case GC_GIRDER_TOP_101:
      case GC_GIRDER_TOP_105:
      case GC_GIRDER_TOP_109:
      case GC_GIRDER_TOP_113:
        return 2.821;
      case GC_STIFFENER_92:
      case GC_STIFFENER_96:
      case GC_STIFFENER_100:
      case GC_STIFFENER_104:
      case GC_STIFFENER_108:
      case GC_STIFFENER_112:
        return 1.209;
      default:
        throw std::invalid_argument("Volume could not be identified in the FD1 ROOT information");
    }
  }

  double fd2_scaling_factor(int gdml) {
    switch (gdml) {
      case NT_1:
        return 1.00000;
      case GC_82:
        return 0.99180;
      case BUFFER_506:
        return 1.0000;
      case NT_TANK:
        return 1.020;
      case NT_GIRDER_34:
      case NT_GIRDER_37:
      case NT_GIRDER_40:
      case NT_GIRDER_43:
      case NT_GIRDER_46:
      case NT_GIRDER_49:
        return 3.431;
      case NT_RING:
        return 1.769;
      case NT_STIFFENER_35:
      case NT_STIFFENER_38:
      case NT_STIFFENER_41:
      case NT_STIFFENER_44:
      case NT_STIFFENER_47:
      case NT_STIFFENER_50:
        return 1.074;
      case GC_TANK:
        return 1.124;
      case GC_GIRDER_BOTTOM_95:
      case GC_GIRDER_BOTTOM_99:
      case GC_GIRDER_BOTTOM_103:
      case GC_GIRDER_BOTTOM_107:
      case GC_GIRDER_BOTTOM_111:
      case GC_GIRDER_BOTTOM_115:
      case GC_GIRDER_MIDDLE_94:
      case GC_GIRDER_MIDDLE_98:
      case GC_GIRDER_MIDDLE_102:
      case GC_GIRDER_MIDDLE_106:
      case GC_GIRDER_MIDDLE_110:
      case GC_GIRDER_MIDDLE_114:
        return 1.047;
      case GC_GIRDER_TOP_93:
      case GC_GIRDER_TOP_97:
      case GC_GIRDER_TOP_101:
      case GC_GIRDER_TOP_105:
      case GC_GIRDER_TOP_109:
      case GC_GIRDER_TOP_113:
        return 2.821;
      case GC_STIFFENER_92:
      case GC_STIFFENER_96:
      case GC_STIFFENER_100:
      case GC_STIFFENER_104:
      case GC_STIFFENER_108:
      case GC_STIFFENER_112:
        return 1.209;
      default:
        throw std::invalid_argument("Volume could not be identified in the FD2 ROOT information");
    }
  }

  ReactorData::ReactorData(std::span<TreeEntry> entries, params::dc::DetectorType type)
    : m_DetectorType(type)
    , m_Evis(entries.size())
    , m_Etrue(entries.size())
    , m_Scaling(entries.size())
    , m_LoverE(entries.size())
    , m_Distance(entries.size()) {
    double (*convert_function)(int);

    auto un_split_type = params::dc::cast_to_no_reactor_split(type);

    if (!params::dc::is_base_type(un_split_type))
      throw std::invalid_argument("The type is not one of the three base types FDI, FDII or ND");

    switch (un_split_type) {
      case params::dc::ND:
        convert_function = &nd_scaling_factor;
        break;
      case params::dc::FDI:
        convert_function = &fd1_scaling_factor;
        break;
      case params::dc::FDII:
        convert_function = &fd2_scaling_factor;
        break;
      default:
        throw std::invalid_argument("Argument could not be handled");
    }

    const unsigned int n_entries = entries.size();

    for (unsigned int i = 0; i < n_entries; ++i) {
      m_Evis[i]     = entries[i].Evis;
      m_Etrue[i]    = entries[i].Etrue;
      m_Scaling[i]  = convert_function(entries[i].GDML);
      m_LoverE[i]   = entries[i].Distance / entries[i].Etrue;
      m_Distance[i] = entries[i].Distance;
    }
  }
} // namespace io