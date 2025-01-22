// js/Logger.js

class Logger {
    constructor(logOutputId) {
        this.logOutputId = logOutputId;
        this.logOutputElement = logOutputId ? document.getElementById(logOutputId) : null;
    }

    debug(...args) { // 新增 debug 方法
        const message = this.formatLog('DEBUG', ...args);
        console.debug(message);
        this.updateLogOutput(message, 'DEBUG');
    }

    info(...args) {
        const message = this.formatLog('INFO', ...args);
        console.info(message);
        this.updateLogOutput(message, 'INFO');
    }

    warn(...args) {
        const message = this.formatLog('WARN', ...args);
        console.warn(message);
        this.updateLogOutput(message, 'WARN');
    }

    error(...args) {
        const message = this.formatLog('ERROR', ...args);
        console.error(message);
        this.updateLogOutput(message, 'ERROR');
    }

    updateLogOutput(message, level) {
        if (!this.logOutputElement) return;

        const logEntry = document.createElement('div');
        logEntry.className = `log-entry log-level-${level.toLowerCase()}`;
        logEntry.textContent = message;

        if (this.logOutputElement.firstChild) {
            this.logOutputElement.insertBefore(logEntry, this.logOutputElement.firstChild);
        } else {
            this.logOutputElement.appendChild(logEntry);
        }
    }

    formatLog(level, ...args) {
        const now = new Date();
        const options = {
            year: 'numeric',
            month: '2-digit',
            day: '2-digit',
            hour: '2-digit',
            minute: '2-digit',
            second: '2-digit'
        };
        const formatter = new Intl.DateTimeFormat('en-US', options);
        const parts = formatter.formatToParts(now);
        const timestampParts = Object.fromEntries(parts.filter(part => part.type !== 'literal').map(part => [part.type, part.value]));
        const timestamp = `${timestampParts.year}-${timestampParts.month}-${timestampParts.day} ${timestampParts.hour}:${timestampParts.minute}:${timestampParts.second}`;

        return `[${timestamp}] - [${level}] ： ${args.map(arg => JSON.stringify(arg)).join(' ')}`;
    }
}

export { Logger };
