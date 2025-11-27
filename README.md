# **AgniVault**

AgniVault is a cybersecurity-focused, systems-level CLI tool that encrypts, stores, opens, and manages entire development projects locally.
All decrypted data exists only in RAM (`/dev/shm/agnivault/`), ensuring **zero plaintext on disk**.

Built in **C++20** using **libsodium**, **yaml-cpp**, and **SQLite**.

---

## **Overview**

AgniVault provides a secure local vault for storing entire project directories. It is designed for developers who want:

- **Local encryption**
- **Offline privacy**
- **Fast RAM-based workspaces**
- **Developer-centric CLI commands**
- **Zero plaintext on disk**

AgniVault uses **identity metadata (name + email)** only for **authenticity tagging**, _not encryption._
Identity must match to open a project, but it is **not** part of any key.

All persistent data is stored under:

```
~/.config/agnivault/
    ├── VaultCode         (master key)
    ├── agnivault.db      (SQLite metadata)
    └── config.yaml       (identity for authenticity checks)
```

Decrypted project files exist only here:

```
/dev/shm/agnivault/<project_name>/
```

---

## **Key Features**

- 🔐 Strong encryption using **libsodium (XChaCha20-Poly1305)**
- 🧾 Identity-based **authenticity tags**, not encryption keys
- ⚡ **RAM-only** decrypted workspace
- 🗄️ SQLite-backed metadata system
- 🛠️ Full CLI for project lifecycle management
- 📁 Handles entire **project directories**, not just secrets
- 🧩 YAML configuration via yaml-cpp
- 🛑 **Zero plaintext ever touches disk**

---

## **Dependencies**

- C++20 compiler (g++ ≥ 11 / clang ≥ 12)
- libsodium
- yaml-cpp
- sqlite3
- make

### **Install (Examples)**

**Debian / Ubuntu**

```bash
sudo apt install libsodium-dev libyaml-cpp-dev libsqlite3-dev build-essential
```

**Arch Linux**

```bash
sudo pacman -S libsodium yaml-cpp sqlite base-devel
```

---

## **Build**

```bash
git clone https://github.com/<you>/AgniVault
cd AgniVault
make          # debug build
make PROD=1   # production build
```

Output:

```
bin/agniv
```

---

## **Configuration**

Location:

```
~/.config/agnivault/config.yaml
```

Example:

```yaml
name: "Your Name"
email: "your@email.com"
```

Identity is embedded into **AEAD associated data (AAD)**:

- ❌ Does _not_ encrypt the file
- ✔️ Prevents decryption if identity mismatches
- ✔️ Acts as authenticity + integrity check

---

## **CLI Usage**

```bash
agniv init <project_name>
```

Create a new project.

```bash
agniv update <old_name> <new_name>
```

Rename an existing project.

```bash
agniv list [project_name]
```

List all projects or show metadata.

```bash
agniv open <project_name>
```

Decrypt project into `/dev/shm/agnivault/<project>`.

```bash
agniv view
```

View open projects (upcoming).

```bash
agniv close <project_name>
```

Securely clear the RAM workspace.

```bash
agniv gen-key
```

Generate a new master key (`VaultCode`).

```bash
agniv config <options>
```

Show or modify `config.yaml`.

---

## **Project Workflow**

### **1. Create a project**

```bash
agniv init myproject
```

### **2. Open the project (decrypts to RAM)**

```bash
agniv open myproject
```

Files appear here:

```
/dev/shm/agnivault/myproject/
```

### **3. Work normally in RAM**

### **4. Close the project**

```bash
agniv close myproject
```

RAM data is securely wiped.

---

## **Security Notes**

- Decrypted files exist only in **RAM (`/dev/shm`)**
- Identity is used only for **authenticity**, not encryption
- Losing the master key = **permanent data loss**
- `/dev/shm` clears automatically on reboot
- Keep permissions secure:

```bash
chmod 600 ~/.config/agnivault/VaultCode
chmod 700 ~/.config/agnivault/
```

---

## **Roadmap**

- Implement `agniv view`
- Key rotation
- Project import/export
- FUSE-based filesystem view
- Sandboxed execution environment
- CLI plugin system
- Auto-encrypt on save
- Extended metadata & tagging

---

## **License**

This project is licensed under the **GNU General Public License (GPL)**.

---
