/*
 * This file is generated by gdbus-codegen, do not modify it.
 *
 * The license of this code is the same as for the D-Bus interface description
 * it was derived from. Note that it links to GLib, so must comply with the
 * LGPL linking clauses.
 */

#ifndef __RAUC_INSTALLER_GENERATED_H__
#define __RAUC_INSTALLER_GENERATED_H__

#include <gio/gio.h>

G_BEGIN_DECLS


/* ------------------------------------------------------------------------ */
/* Declarations for de.pengutronix.rauc.Installer */

#define R_TYPE_INSTALLER (r_installer_get_type ())
#define R_INSTALLER(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), R_TYPE_INSTALLER, RInstaller))
#define R_IS_INSTALLER(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), R_TYPE_INSTALLER))
#define R_INSTALLER_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), R_TYPE_INSTALLER, RInstallerIface))

struct _RInstaller;
typedef struct _RInstaller RInstaller;
typedef struct _RInstallerIface RInstallerIface;

struct _RInstallerIface
{
  GTypeInterface parent_iface;



  gboolean (*handle_get_primary) (
    RInstaller *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_get_slot_status) (
    RInstaller *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_info) (
    RInstaller *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_bundle);

  gboolean (*handle_inspect_bundle) (
    RInstaller *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_source,
    GVariant *arg_args);

  gboolean (*handle_install) (
    RInstaller *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_source);

  gboolean (*handle_install_bundle) (
    RInstaller *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_source,
    GVariant *arg_args);

  gboolean (*handle_mark) (
    RInstaller *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_state,
    const gchar *arg_slot_identifier);

  const gchar * (*get_boot_slot) (RInstaller *object);

  const gchar * (*get_compatible) (RInstaller *object);

  const gchar * (*get_last_error) (RInstaller *object);

  const gchar * (*get_operation) (RInstaller *object);

  GVariant * (*get_progress) (RInstaller *object);

  const gchar * (*get_variant) (RInstaller *object);

  void (*completed) (
    RInstaller *object,
    gint arg_result);

};

GType r_installer_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *r_installer_interface_info (void);
guint r_installer_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void r_installer_complete_install (
    RInstaller *object,
    GDBusMethodInvocation *invocation);

void r_installer_complete_install_bundle (
    RInstaller *object,
    GDBusMethodInvocation *invocation);

void r_installer_complete_info (
    RInstaller *object,
    GDBusMethodInvocation *invocation,
    const gchar *compatible,
    const gchar *version);

void r_installer_complete_inspect_bundle (
    RInstaller *object,
    GDBusMethodInvocation *invocation,
    GVariant *info);

void r_installer_complete_mark (
    RInstaller *object,
    GDBusMethodInvocation *invocation,
    const gchar *slot_name,
    const gchar *message);

void r_installer_complete_get_slot_status (
    RInstaller *object,
    GDBusMethodInvocation *invocation,
    GVariant *slot_status_array);

void r_installer_complete_get_primary (
    RInstaller *object,
    GDBusMethodInvocation *invocation,
    const gchar *primary);



/* D-Bus signal emissions functions: */
void r_installer_emit_completed (
    RInstaller *object,
    gint arg_result);



/* D-Bus method calls: */
void r_installer_call_install (
    RInstaller *proxy,
    const gchar *arg_source,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean r_installer_call_install_finish (
    RInstaller *proxy,
    GAsyncResult *res,
    GError **error);

gboolean r_installer_call_install_sync (
    RInstaller *proxy,
    const gchar *arg_source,
    GCancellable *cancellable,
    GError **error);

void r_installer_call_install_bundle (
    RInstaller *proxy,
    const gchar *arg_source,
    GVariant *arg_args,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean r_installer_call_install_bundle_finish (
    RInstaller *proxy,
    GAsyncResult *res,
    GError **error);

gboolean r_installer_call_install_bundle_sync (
    RInstaller *proxy,
    const gchar *arg_source,
    GVariant *arg_args,
    GCancellable *cancellable,
    GError **error);

void r_installer_call_info (
    RInstaller *proxy,
    const gchar *arg_bundle,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean r_installer_call_info_finish (
    RInstaller *proxy,
    gchar **out_compatible,
    gchar **out_version,
    GAsyncResult *res,
    GError **error);

gboolean r_installer_call_info_sync (
    RInstaller *proxy,
    const gchar *arg_bundle,
    gchar **out_compatible,
    gchar **out_version,
    GCancellable *cancellable,
    GError **error);

void r_installer_call_inspect_bundle (
    RInstaller *proxy,
    const gchar *arg_source,
    GVariant *arg_args,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean r_installer_call_inspect_bundle_finish (
    RInstaller *proxy,
    GVariant **out_info,
    GAsyncResult *res,
    GError **error);

gboolean r_installer_call_inspect_bundle_sync (
    RInstaller *proxy,
    const gchar *arg_source,
    GVariant *arg_args,
    GVariant **out_info,
    GCancellable *cancellable,
    GError **error);

void r_installer_call_mark (
    RInstaller *proxy,
    const gchar *arg_state,
    const gchar *arg_slot_identifier,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean r_installer_call_mark_finish (
    RInstaller *proxy,
    gchar **out_slot_name,
    gchar **out_message,
    GAsyncResult *res,
    GError **error);

gboolean r_installer_call_mark_sync (
    RInstaller *proxy,
    const gchar *arg_state,
    const gchar *arg_slot_identifier,
    gchar **out_slot_name,
    gchar **out_message,
    GCancellable *cancellable,
    GError **error);

void r_installer_call_get_slot_status (
    RInstaller *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean r_installer_call_get_slot_status_finish (
    RInstaller *proxy,
    GVariant **out_slot_status_array,
    GAsyncResult *res,
    GError **error);

gboolean r_installer_call_get_slot_status_sync (
    RInstaller *proxy,
    GVariant **out_slot_status_array,
    GCancellable *cancellable,
    GError **error);

void r_installer_call_get_primary (
    RInstaller *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean r_installer_call_get_primary_finish (
    RInstaller *proxy,
    gchar **out_primary,
    GAsyncResult *res,
    GError **error);

gboolean r_installer_call_get_primary_sync (
    RInstaller *proxy,
    gchar **out_primary,
    GCancellable *cancellable,
    GError **error);



/* D-Bus property accessors: */
const gchar *r_installer_get_operation (RInstaller *object);
gchar *r_installer_dup_operation (RInstaller *object);
void r_installer_set_operation (RInstaller *object, const gchar *value);

const gchar *r_installer_get_last_error (RInstaller *object);
gchar *r_installer_dup_last_error (RInstaller *object);
void r_installer_set_last_error (RInstaller *object, const gchar *value);

GVariant *r_installer_get_progress (RInstaller *object);
GVariant *r_installer_dup_progress (RInstaller *object);
void r_installer_set_progress (RInstaller *object, GVariant *value);

const gchar *r_installer_get_compatible (RInstaller *object);
gchar *r_installer_dup_compatible (RInstaller *object);
void r_installer_set_compatible (RInstaller *object, const gchar *value);

const gchar *r_installer_get_variant (RInstaller *object);
gchar *r_installer_dup_variant (RInstaller *object);
void r_installer_set_variant (RInstaller *object, const gchar *value);

const gchar *r_installer_get_boot_slot (RInstaller *object);
gchar *r_installer_dup_boot_slot (RInstaller *object);
void r_installer_set_boot_slot (RInstaller *object, const gchar *value);


/* ---- */

#define R_TYPE_INSTALLER_PROXY (r_installer_proxy_get_type ())
#define R_INSTALLER_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), R_TYPE_INSTALLER_PROXY, RInstallerProxy))
#define R_INSTALLER_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), R_TYPE_INSTALLER_PROXY, RInstallerProxyClass))
#define R_INSTALLER_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), R_TYPE_INSTALLER_PROXY, RInstallerProxyClass))
#define R_IS_INSTALLER_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), R_TYPE_INSTALLER_PROXY))
#define R_IS_INSTALLER_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), R_TYPE_INSTALLER_PROXY))

typedef struct _RInstallerProxy RInstallerProxy;
typedef struct _RInstallerProxyClass RInstallerProxyClass;
typedef struct _RInstallerProxyPrivate RInstallerProxyPrivate;

struct _RInstallerProxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  RInstallerProxyPrivate *priv;
};

struct _RInstallerProxyClass
{
  GDBusProxyClass parent_class;
};

GType r_installer_proxy_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (RInstallerProxy, g_object_unref)
#endif

void r_installer_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
RInstaller *r_installer_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
RInstaller *r_installer_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void r_installer_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
RInstaller *r_installer_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
RInstaller *r_installer_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define R_TYPE_INSTALLER_SKELETON (r_installer_skeleton_get_type ())
#define R_INSTALLER_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), R_TYPE_INSTALLER_SKELETON, RInstallerSkeleton))
#define R_INSTALLER_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), R_TYPE_INSTALLER_SKELETON, RInstallerSkeletonClass))
#define R_INSTALLER_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), R_TYPE_INSTALLER_SKELETON, RInstallerSkeletonClass))
#define R_IS_INSTALLER_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), R_TYPE_INSTALLER_SKELETON))
#define R_IS_INSTALLER_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), R_TYPE_INSTALLER_SKELETON))

typedef struct _RInstallerSkeleton RInstallerSkeleton;
typedef struct _RInstallerSkeletonClass RInstallerSkeletonClass;
typedef struct _RInstallerSkeletonPrivate RInstallerSkeletonPrivate;

struct _RInstallerSkeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  RInstallerSkeletonPrivate *priv;
};

struct _RInstallerSkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType r_installer_skeleton_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (RInstallerSkeleton, g_object_unref)
#endif

RInstaller *r_installer_skeleton_new (void);


G_END_DECLS

#endif /* __RAUC_INSTALLER_GENERATED_H__ */
