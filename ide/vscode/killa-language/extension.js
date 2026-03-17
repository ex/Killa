const vscode = require('vscode');
const { execSync } = require('child_process');

function findKilla() {
    try {
        const cmd = process.platform === 'win32' ? 'where killa' : 'which killa';
        const result = execSync(cmd, { encoding: 'utf8' }).trim();
        return result.split('\n')[0].trim();
    } catch {
        return null;
    }
}

let terminal = null;

function activate(context) {
    const runFile = vscode.commands.registerCommand('killa.runFile', () => {
        const editor = vscode.window.activeTextEditor;

        if (!editor) {
            vscode.window.showErrorMessage('Killa: No active file to run.');
            return;
        }

        if (editor.document.languageId !== 'killa') {
            vscode.window.showErrorMessage('Killa: Active file is not a .kia script.');
            return;
        }

        const killaPath = findKilla();
        if (!killaPath) {
            vscode.window.showErrorMessage(
                'Killa: "killa" executable not found in PATH. ' +
                'Add killa.exe to your PATH and reload the window.'
            );
            return;
        }

        editor.document.save().then(() => {
            const filePath = editor.document.fileName;

            if (!terminal || terminal.exitStatus !== undefined) {
                terminal = vscode.window.createTerminal('Killa');
            }

            terminal.show(true);
            terminal.sendText(`"${killaPath}" "${filePath}"`);
        });
    });

    context.subscriptions.push(runFile);

    context.subscriptions.push(
        vscode.window.onDidCloseTerminal(t => {
            if (t === terminal) {
                terminal = null;
            }
        })
    );
}

function deactivate() {}

module.exports = { activate, deactivate };
